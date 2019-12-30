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
#include "Scope/Control/AnnounceStorage.h"

#include "Scope/Serialisation/Protobuf/NanopbPacker.h"
#include "Scope/Control/IPacker.h"
#include "Scope/Core/ScopeTypes.h"
#include "Scope/GeneralPurpose/FloatRingBuffer.h"
#include "Scope/GeneralPurpose/IIntStream.h"
#include "Scope/Version.h"
#include "Scope/Serialisation/Protobuf/se-scope.pb.h"

#include <assert.h>
#include <pb.h>
#include <stdlib.h>
#include <pb_encode.h>

#define FLOWCONTROL_BUFFER_SIZE 30
#define TINC_BUFFER_SIZE 30
#define TRIGGER_BUFFER_SIZE 60
#define MAX_CONTROL_SIGN_SIZE 30

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __ChannelDef {
    FloatRingBufferHandle stream;
    uint32_t id;
} ChannelDef;

typedef struct __ChannelsDef {
    ChannelDef* channels;
    size_t amountOfChannels;
} ChannelsDef;

typedef struct __TriggerDef {
    bool isTriggered;
    uint32_t channelId;
    uint32_t timestamp;
    TRIGGER_MODE triggerMode;
} TriggerDef;

typedef struct __NanopbPackerPrivateData {

    IPacker packer;
    pb_ostream_t wrapped;
    ChannelsDef channels;
    size_t maxNumberOfChannels;
    TriggerDef trigger;
    uint32_t timeIncrement;
    IIntStreamHandle timestamp;

} NanopbPackerPrivateData;

static void addChannel(IPackerHandle packer, FloatRingBufferHandle buffer, const uint32_t channelId);

static void addTimeIncrement(IPackerHandle packer, const uint32_t timeIncrement);

static void addTrigger(IPackerHandle packer, const bool isTriggered, const uint32_t channelId, const uint32_t timestamp,
                       TRIGGER_MODE triggerMode);

static void addTimestamp(IPackerHandle packer, IIntStreamHandle timestamp);

static void
addAddressAnnouncement(IPackerHandle packer, const char *name, const char *type, ADDRESS_DATA_TYPE address);

static void
addAnnouncement(IPackerHandle packer, float timeBase, const char *version, size_t maxChannels);

static void reset(IPackerHandle packer);

static bool isReady(IPackerHandle packer);

static bool writeStreamCallback(pb_ostream_t *stream, const pb_byte_t *buf, size_t count);

static void pack(IPackerHandle packer, MessageType type);

static size_t getWidthOfVarint(uint32_t data);
/******************************************************************************
 Private functions
******************************************************************************/
static void addChannel(IPackerHandle packer, FloatRingBufferHandle buffer, const uint32_t channelId) {
    NanopbPackerHandle self = (NanopbPackerHandle) packer->handle;
    ChannelDef channel;
    channel.stream = buffer;
    channel.id = channelId;
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

static void addTrigger(IPackerHandle packer, const bool isTriggered, const uint32_t channelId, const uint32_t timestamp,
               TRIGGER_MODE triggerMode) {
    NanopbPackerHandle self = (NanopbPackerHandle) packer->handle;
    self->trigger.channelId = channelId;
    self->trigger.isTriggered = isTriggered;
    self->trigger.timestamp = timestamp;
    self->trigger.triggerMode = triggerMode;
}

static void
addAddressAnnouncement(IPackerHandle packer, const char *name, const char *type, ADDRESS_DATA_TYPE address) {
    NanopbPackerHandle self = (NanopbPackerHandle) packer->handle;

}

static void addAnnouncement(IPackerHandle packer, float timeBase, const char *version, size_t maxChannels){
    NanopbPackerHandle self = (NanopbPackerHandle) packer->handle;

}

static void reset(IPackerHandle packer) {
    NanopbPackerHandle self = (NanopbPackerHandle) packer->handle;
}

static bool isReady(IPackerHandle packer){
    NanopbPackerHandle self = (NanopbPackerHandle) packer->handle;
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
    ChannelDef channel = *(ChannelDef*) (*arg);
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
    ChannelsDef channels = *(ChannelsDef*) (*arg);
    SC_Channel channel = SC_Channel_init_zero;

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

        if(!pb_encode(stream, SC_Channel_fields, &channel)){
            return false;
        };
    }
    return true;
}

static void packScData(NanopbPackerHandle self){

    SC_Data data = SC_Data_init_zero;

    if(self->trigger.isTriggered){
        data.has_trigger = true;
        data.trigger.cl_data_ind = self->trigger.timestamp;
        data.trigger.cl_id = self->trigger.channelId;
        data.trigger.mode  = (SC_Trigger_Mode) self->trigger.triggerMode;
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

    pb_encode(&self->wrapped, SC_Data_fields, &data);

}

static void pack(IPackerHandle packer, MessageType type){
    NanopbPackerHandle self = (NanopbPackerHandle) packer->handle;

    if(type == SC_DATA) {
        packScData(self);
    }
}

/******************************************************************************
 Public functions
******************************************************************************/
NanopbPackerHandle NanopbPacker_create(size_t maxNumberOfChannels, size_t maxAddressesToAnnounce,
                                   IByteStreamHandle output) {

    NanopbPackerHandle self = (NanopbPackerHandle) malloc(sizeof(NanopbPackerPrivateData));

    self->wrapped.state = output;
    self->wrapped.callback = &writeStreamCallback;
    self->wrapped.max_size = output->capacity(output);
    self->wrapped.bytes_written = 0;

    self->channels.channels = malloc(sizeof(ChannelDef) * maxNumberOfChannels);
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
    // TODO: ACTUALLY calulate this...
    return 2000;
}

IPackerHandle NanopbPacker_getIPacker(NanopbPackerHandle self) {
    return &self->packer;
}
