/*!****************************************************************************************************************************************
 * @file         NanopbPacker.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include "Scope/GeneralPurpose/IByteStream.h"
#include "Scope/GeneralPurpose/DataTypes.h"

#include "Scope/Serialisation/Protobuf/NanopbPacker.h"
#include "Scope/Control/IPacker.h"
#include "Scope/Core/ScopeTypes.h"
#include "Scope/GeneralPurpose/FloatRingBuffer.h"
#include "Scope/GeneralPurpose/IIntStream.h"
#include "Scope/Serialisation/Protobuf/se-scope.pb.h"

#include <assert.h>
#include <pb.h>
#include <stdlib.h>
#include <pb_encode.h>

typedef struct __ScDataChannelsDef {
    ScDataChannelDef* channels;
    size_t amountOfChannels;
} ScDataChannelsDef;

/******************************************************************************
 Define private data
******************************************************************************/
typedef struct __NanopbPackerPrivateData {

    IPacker packer;
    pb_ostream_t wrapped;
    ScDataChannelsDef channels;
    size_t maxNumberOfChannels;
    ScDataTriggerDef trigger;
    uint32_t timeIncrement;
    IIntStreamHandle timestamp;
		IByteStreamHandle output;

} NanopbPackerPrivateData;

static void addChannel(IPackerHandle packer, ScDataChannelDef channel);

static void addTimeIncrement(IPackerHandle packer, const uint32_t timeIncrement);

static void addTrigger(IPackerHandle packer, ScDataTriggerDef trigger);

static void addTimestamp(IPackerHandle packer, IIntStreamHandle timestamp);

static void
addAddressAnnouncement(IPackerHandle packer, const char *name, const char *type, ADDRESS_DATA_TYPE address);

static void
addAnnouncement(IPackerHandle packer, float timeBase, const char *version, size_t maxChannels);

static void reset(IPackerHandle packer);

static void resetSelf(NanopbPackerHandle self);

static bool isReady(IPackerHandle packer);

static bool writeStreamCallback(pb_ostream_t *stream, const pb_byte_t *buf, size_t count);

static void pack(IPackerHandle packer, MessageType type);

static size_t getWidthOfVarint(uint32_t data);
/******************************************************************************
 Private functions
******************************************************************************/
static void addChannel(IPackerHandle packer, ScDataChannelDef channel) {
    NanopbPackerHandle self = (NanopbPackerHandle) packer->handle;
    self->channels.channels[self->channels.amountOfChannels] = channel;
    self->channels.amountOfChannels += 1;
}

static void addTimeIncrement(IPackerHandle packer, const uint32_t timeIncrement) {
    NanopbPackerHandle self = (NanopbPackerHandle) packer->handle;
    self->timeIncrement = timeIncrement;
}

static void addTimestamp(IPackerHandle packer, IIntStreamHandle timestamp) {
    NanopbPackerHandle self = (NanopbPackerHandle) packer->handle;
    self->timestamp = timestamp;
}

static void addTrigger(IPackerHandle packer, ScDataTriggerDef trigger) {
    NanopbPackerHandle self = (NanopbPackerHandle) packer->handle;
    self->trigger = trigger;
}

static void
addAddressAnnouncement(IPackerHandle packer, const char *name, const char *type, ADDRESS_DATA_TYPE address) {
    NanopbPackerHandle self = (NanopbPackerHandle) packer->handle;
    // Not yet implemented for nanopb
    return;
}

static void addAnnouncement(IPackerHandle packer, float timeBase, const char *version, size_t maxChannels){
    NanopbPackerHandle self = (NanopbPackerHandle) packer->handle;
    // Not yet implemented for nanopb
    return;
}

static void reset(IPackerHandle packer) {
    NanopbPackerHandle self = (NanopbPackerHandle) packer->handle;
    resetSelf(self);
}

static bool isReady(IPackerHandle packer){
    NanopbPackerHandle self = (NanopbPackerHandle) packer->handle;
    return self->wrapped.bytes_written == 0;
}

static bool writeStreamCallback(pb_ostream_t *stream, const pb_byte_t *buf, size_t count){
    IByteStreamHandle byteStream = (IByteStreamHandle) stream->state;
    byteStream->write(byteStream, buf, count);
    return true;
}

static size_t getWidthOfVarint(uint32_t data){
    for(size_t i = 1; i < 4; i++){
        if(data >> i * 4 == 0){
            return i;
        }
    }
    return 1;
}

static bool writeTimestamp(pb_ostream_t* stream, const pb_field_t* field, void* const* arg){
    IIntStreamHandle timestamp = (IIntStreamHandle) (*arg);
    size_t length = timestamp->length(timestamp);

    if(!pb_encode_tag(stream, PB_WT_STRING, field->tag))
        return false;

    if(!pb_encode_varint(stream, length * 4))
        return false;

    for(size_t i = 0; i < length; i++){
        uint32_t data = timestamp->readData(timestamp);
        if(pb_encode_fixed32(stream, &data) == false){
            return false;
        };
    }
    return true;
}

static bool writeChannelData(pb_ostream_t* stream, const pb_field_t* field, void* const* arg){
    ScDataChannelDef channel = *(ScDataChannelDef*) (*arg);
    size_t length = FloatRingBuffer_getNumberOfUsedData(channel.stream);

    if(!pb_encode_tag(stream, PB_WT_STRING, field->tag))
        return false;

    if(!pb_encode_varint(stream, length * 4))
        return false;

    for(size_t i = 0; i < length; i++){
        float data;
        FloatRingBuffer_read(channel.stream, &data, 1);

        if(pb_encode_fixed32(stream, &data) == false){
            return false;
        };
    }
    return true;
}

static bool writeChannel(pb_ostream_t* stream, const pb_field_t* field, void* const* arg){
    ScDataChannelsDef channels = *(ScDataChannelsDef*) (*arg);
    PB_SC_Channel channel = PB_SC_Channel_init_zero;

    for(size_t i = 0; i < channels.amountOfChannels; i++){
        channel.data.arg = &channels.channels[i];
        channel.data.funcs.encode = &writeChannelData;
        channel.id = channels.channels[i].id;

        if(!pb_encode_tag(stream, PB_WT_STRING, field->tag))
            return false;

        size_t length = FloatRingBuffer_getNumberOfUsedData(channels.channels[i].stream);

        // Length of data: length * 4 for float data
        // channel data wiretype + field id = 1
        // inner length of data calculated with getWidthOfVarint()

        // Id is only included when != 0
        // channel id in varint calculated with getWidthOfVarint()
        // channel id wiretype + field id = 1
        size_t idSize = 0;
        if(channels.channels[i].id != 0){
            idSize = 1 + getWidthOfVarint(channels.channels[i].id);
        }

        if(!pb_encode_varint(stream, length * 4 + 1 + idSize + getWidthOfVarint(length)))
            return false;

        if(!pb_encode(stream, PB_SC_Channel_fields, &channel)){
            return false;
        };
    }
    return true;
}

static void packScData(NanopbPackerHandle self){

    PB_SC_Data data = PB_SC_Data_init_zero;

    if(self->trigger.isTriggered){
        data.has_trigger = true;
        data.trigger.cl_data_ind = self->trigger.timestamp;
        data.trigger.cl_id = self->trigger.channelId;
        data.trigger.mode  = (PB_Trigger_Mode) self->trigger.triggerMode;
    }

    data.t_inc = self->timeIncrement;

    if(self->timestamp != NULL){
        data.timestamps.funcs.encode = &writeTimestamp;
        data.timestamps.arg = self->timestamp;
    }

    if(self->channels.amountOfChannels > 0){
        data.channels.arg = &self->channels;
        data.channels.funcs.encode = &writeChannel;
    }

    pb_encode(&self->wrapped, PB_SC_Data_fields, &data);

}

static void resetSelf(NanopbPackerHandle self){
    self->channels.amountOfChannels = 0;
		self->wrapped.bytes_written = 0;
}

static void pack(IPackerHandle packer, MessageType type){
    NanopbPackerHandle self = (NanopbPackerHandle) packer->handle;

    if(self->output->length(self->output) != 0) {
        return;
    }
	
    if(type == SC_DATA) {
        packScData(self);
    }
		
    resetSelf(self);
}

/******************************************************************************
 Public functions
******************************************************************************/
NanopbPackerHandle NanopbPacker_create(size_t maxNumberOfChannels, size_t maxAddressesToAnnounce,
                                   IByteStreamHandle output) {

    NanopbPackerHandle self = (NanopbPackerHandle) malloc(sizeof(NanopbPackerPrivateData));

    assert(self);

    self->wrapped.state = output;
    self->wrapped.callback = &writeStreamCallback;
    self->wrapped.max_size = output->capacity(output);
    self->wrapped.bytes_written = 0;

    self->output = output;
    self->channels.channels = malloc(sizeof(ScDataChannelDef) * maxNumberOfChannels);

    assert(self->channels.channels);

    self->maxNumberOfChannels = maxNumberOfChannels;

    self->packer.handle = self;
    self->packer.addChannel = &addChannel;
    self->packer.addTimeIncrement = &addTimeIncrement;
    self->packer.addTimestamp = &addTimestamp;
    self->packer.addTrigger = &addTrigger;
    self->packer.addAddressAnnouncement = &addAddressAnnouncement;
    self->packer.addAnnouncement = &addAnnouncement;
    self->packer.pack = &pack;
    self->packer.isReady = &isReady;
    self->packer.reset = &reset;

    return self;
}

void NanopbPacker_destroy(NanopbPackerHandle self) {
    free(self);
    self = NULL;
}

size_t
NanopbPacker_calculateBufferSize(size_t maxNumberOfChannels, size_t sizeOfChannels, size_t maxAddressesToAnnounce) {
    // Wiretypes and id will always only take up 1 byte. Wiretypes and Index are indicated with WI in the comments.
    // Actual data will always max take up 4 bytes -> 32 bits.

    // ChannelWI + ArrWI + IdWi + ID + ChannelSize + ArrSize
    size_t channelMetaData = 1 + 1 + 1 + (1 + 1 + 1) * 4; // -> 3 Byte overhead per channel
    size_t dataSpace = (sizeOfChannels * 4 + channelMetaData) * maxNumberOfChannels;
    size_t triggerSize = PB_SC_Trigger_size;
    // Data + WI
    size_t tIncSize = 1 * 4 + 1;
    // Data + Arrlenght + WI
    size_t timestampSize = (sizeOfChannels + 1) * 4 + 1;
    return dataSpace + triggerSize + tIncSize + timestampSize;
}

IPackerHandle NanopbPacker_getIPacker(NanopbPackerHandle self) {
    return &self->packer;
}
