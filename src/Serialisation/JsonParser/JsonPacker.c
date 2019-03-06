/*!****************************************************************************************************************************************
 * @file         JsonPacker.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Serialisation/JsonParser/JsonPacker.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <Scope/Serialisation/JsonParser/JsonCommon.h>
#include <Scope/Control/ParserDefinitions.h>

#define FLOWCONTROL_BUFFER_SIZE 30
#define TINC_BUFFER_SIZE 30
#define TRIGGER_BUFFER_SIZE 60
#define MAX_CONTROL_SIGN_SIZE 30

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __JsonPackerPrivateData{
    IPacker packer;
    size_t maxNumberOfChannels;

    /* Data for the Announce address feature */
    size_t maxAddressesToAnnounce;
    char** namesOfAddresses;
    char** typesOfAddresses;
    ADDRESS_DATA_TYPE* addresses;
    uint32_t numberOfAddressesToAnnounce;
    bool addressesReady;
    IByteStreamHandle byteStream;

    /* Channel preparation data */
    bool channelsReady;
    size_t amountOfChannelsToSend;
    uint32_t* channelIds;
    FloatRingBufferHandle* channelBuffers;

    /* Timestamp increment data */
    bool tIncReady;
    uint32_t timeIncrement;

    /* Timestamp data */
    bool timestampReady;
    IIntStreamHandle timestamp;

    /* Trigger data */
    bool triggerReady;
    bool isTriggered;
    uint32_t channelId;
    uint32_t triggerTimestamp;

    /* Flow control data */
    bool flowcontrolReady;
    char flowcontrol[4];
    bool dataPendingToBePacked;

} JsonPackerPrivateData;

inline static void appendString(IByteStreamHandle destination, const char* origin, size_t originSize,
                                const char* endWith, size_t endWidthSize);

static void appendNumber(IByteStreamHandle destination, ADDRESS_DATA_TYPE origin, const char* endWith,
                         size_t endWithSize);

static void flushBuffer(char* buffer);

static void addComma(IByteStreamHandle destination, bool commaIsNeeded);

static bool channelMapIsEmpty(JsonPackerHandle self);

static bool packTimeIncrement(JsonPackerHandle self, bool commaIsNeeded);

static void prepareTimestamp(IPackerHandle packer, IIntStreamHandle timestamp);

static bool packTimestamp(JsonPackerHandle self, bool commaIsNeeded);

static bool packTrigger(JsonPackerHandle self, bool commaIsNeeded);

static bool packFlowControl(JsonPackerHandle self, bool commaIsNeeded);

static void
prepareAddressAnnouncement(IPackerHandle packer, const char* name, const char* type, ADDRESS_DATA_TYPE address);

static bool packAddressAnnouncement(JsonPackerHandle self, bool commaIsNeeded);

static bool packChannel(JsonPackerHandle self, bool commaIsNeeded);

static bool packChannelMap(JsonPackerHandle self);

static bool packPayloadMap(JsonPackerHandle self);

static void appendFloat(float data, IByteStreamHandle stream);

/******************************************************************************
 Private functions
******************************************************************************/
inline static void appendFloat(float data, IByteStreamHandle stream){

    char buf[MAX_LENGTH_OF_NUMBER];
    int length = sprintf(buf, "%.4g", data);
    stream->write(stream, (uint8_t*) buf, (size_t) length);
}

inline static void appendString(IByteStreamHandle destination, const char* origin, size_t originSize,
                                const char* endWith, size_t endWidthSize){
    destination->writeByte(destination, (uint8_t) '\"');
    destination->write(destination, (uint8_t*) origin, originSize);
    destination->writeByte(destination, (uint8_t) '\"');

    if(endWidthSize != 0){
        destination->write(destination, (uint8_t*) endWith, endWidthSize);
    }
}

inline static void appendData(IByteStreamHandle destination, const char* origin, size_t originSize,
                              const char* endWith, size_t endWidthSize){
    destination->write(destination, (uint8_t*) origin, originSize);
    if(endWidthSize != 0){
        destination->write(destination, (uint8_t*) endWith, endWidthSize);
    }
}

inline static void flushBuffer(char* buffer){
    buffer[0] = '\0';
}

inline static void addComma(IByteStreamHandle destination, bool commaIsNeeded){

    if(commaIsNeeded == true){
        appendData(destination, ",", 1, "", 0);
    }
}

inline static void appendNumber(IByteStreamHandle destination, ADDRESS_DATA_TYPE origin, const char* endWith,
                                size_t endWithSize){


    char buffer[MAX_LENGTH_OF_NUMBER];
    size_t length = 0;
#if (ARCH_SIZE_32)
    length = sprintf(buffer, "%u", (unsigned int) origin);
#else
    length = sprintf(buffer, "%llu", (long long unsigned int) origin);
#endif
    destination->write(destination, (uint8_t*) buffer, length);

    if(endWithSize != 0){
        destination->write(destination, (uint8_t*) endWith, endWithSize);
    }
}

static void prepareChannel(IPackerHandle packer, FloatRingBufferHandle buffer, const uint32_t channelId){
    JsonPackerHandle self = (JsonPackerHandle) packer->handle;

    if(channelId >= self->maxNumberOfChannels){
        return;
    }

    if(self->amountOfChannelsToSend >= self->maxNumberOfChannels){
        return;
    }

    self->channelIds[self->amountOfChannelsToSend] = channelId;
    self->channelBuffers[self->amountOfChannelsToSend] = buffer;

    self->amountOfChannelsToSend++;

    self->channelsReady = true;
    self->dataPendingToBePacked = true;
}

static void prepareTimeIncrement(IPackerHandle packer, const uint32_t timeIncrement){
    JsonPackerHandle self = (JsonPackerHandle) packer->handle;

    self->tIncReady = true;
    self->timeIncrement = timeIncrement;

    self->dataPendingToBePacked = true;
}

static bool packTimeIncrement(JsonPackerHandle self, bool commaIsNeeded){

    if(self->tIncReady == false){
        return commaIsNeeded;
    }

    addComma(self->byteStream, commaIsNeeded);
    appendString(self->byteStream, KEYWORD_T_INC, KEYWORD_T_INC_LENGTH, ":", 1);
    appendNumber(self->byteStream, self->timeIncrement, "", 0);

    self->tIncReady = false;

    return true;
}


static void prepareTimestamp(IPackerHandle packer, IIntStreamHandle timestamp){
    JsonPackerHandle self = (JsonPackerHandle) packer->handle;

    self->timestampReady = true;
    self->timestamp = timestamp;
    self->dataPendingToBePacked = true;
}

static bool packTimestamp(JsonPackerHandle self, bool commaIsNeeded){

    if(self->timestampReady == false){
        return commaIsNeeded;
    }

    if(self->timestamp == NULL){
        return commaIsNeeded;
    }

    addComma(self->byteStream, commaIsNeeded);
    appendString(self->byteStream, KEYWORD_T_STMP, KEYWORD_T_STMP_LENGTH, ":[", 2);


    uint32_t data;
    const size_t dataLength = self->timestamp->length(self->timestamp);

    data = self->timestamp->readData(self->timestamp);
    appendNumber(self->byteStream, data, "", 0);

    for(int i = 1; i < dataLength; ++i){

        self->byteStream->writeByte(self->byteStream, ',');
        data = self->timestamp->readData(self->timestamp);
        appendNumber(self->byteStream, data, "", 0);
    }
    appendData(self->byteStream, "]", 1, "", 0);

    self->timestampReady = false;

    return true;
}

static void
prepareTrigger(IPackerHandle packer, const bool isTriggered, const uint32_t channelId, const uint32_t timestamp){
    JsonPackerHandle self = (JsonPackerHandle) packer->handle;

    self->triggerReady = true;
    self->isTriggered = isTriggered;
    self->channelId = channelId;
    self->triggerTimestamp = timestamp;
    self->dataPendingToBePacked = true;
}

static bool packTrigger(JsonPackerHandle self, bool commaIsNeeded){

    if(self->triggerReady == false){
        return commaIsNeeded;
    }

    addComma(self->byteStream, commaIsNeeded);
    appendString(self->byteStream, KEYWORD_TGR, KEYWORD_TGR_LENGTH, ":{", 2);

    appendString(self->byteStream, KEYWORD_TGR_FOUND, KEYWORD_TGR_FOUND_LENGTH, ":", 1);

    if(self->isTriggered == true){
        appendData(self->byteStream, KEYWORD_TRUE, KEYWORD_TRUE_LENGTH, ",", 1);

        appendString(self->byteStream, KEYWORD_TGR_CL_DATA_IND, KEYWORD_TGR_CL_DATA_IND_LENGTH, ":", 1);
        appendNumber(self->byteStream, self->triggerTimestamp, ",", 1);

        appendString(self->byteStream, KEYWORD_TGR_CL_ID, KEYWORD_TGR_CL_ID_LENGTH, ":", 1);
        appendNumber(self->byteStream, self->channelId, "}", 1);

    }else{
        appendData(self->byteStream, KEYWORD_FALSE, KEYWORD_FALSE_LENGTH, "}", 1);
    }

    self->triggerReady = false;

    return true;
}

static void prepareFlowControl(IPackerHandle packer, const char* flowControl){
    JsonPackerHandle self = (JsonPackerHandle) packer->handle;

    /* Flow control data will be "NAK" or "ACK" which is why longer strings will be rejected by default */
    if(strlen(flowControl) > 3){
        return;
    }

    self->flowcontrolReady = true;
    strcpy(self->flowcontrol, flowControl);
    self->dataPendingToBePacked = true;
}

static bool packFlowControl(JsonPackerHandle self, bool commaIsNeeded){

    if(self->flowcontrolReady == false){
        return commaIsNeeded;
    }

    addComma(self->byteStream, commaIsNeeded);
    appendString(self->byteStream, KEYWORD_FLOW_CTRL, KEYWORD_FLOW_CTRL_LENGTH, ":", 1);
    appendString(self->byteStream, self->flowcontrol, strlen(self->flowcontrol), "", 0);

    self->flowcontrolReady = false;

    return true;
}


static void
prepareAddressAnnouncement(IPackerHandle packer, const char* name, const char* type, ADDRESS_DATA_TYPE address){
    JsonPackerHandle self = (JsonPackerHandle) packer->handle;

    if(self->numberOfAddressesToAnnounce >= self->maxAddressesToAnnounce){
        return;
    }

    self->addresses[self->numberOfAddressesToAnnounce] = address;
    self->namesOfAddresses[self->numberOfAddressesToAnnounce] = (char*) name;
    self->typesOfAddresses[self->numberOfAddressesToAnnounce] = (char*) type;

    self->numberOfAddressesToAnnounce++;

    self->addressesReady = true;
    self->dataPendingToBePacked = true;
}

static bool packAddressAnnouncement(JsonPackerHandle self, bool commaIsNeeded){

    if(self->addressesReady == false){
        return commaIsNeeded;
    }

    if(self->numberOfAddressesToAnnounce == 0){
        return commaIsNeeded;
    }

    addComma(self->byteStream, commaIsNeeded);
    appendString(self->byteStream, KEYWORD_ANNOUNCE, KEYWORD_ANNOUNCE_LENGTH, ":{", 2);

    for(size_t i = 0; i < self->numberOfAddressesToAnnounce; ++i){

        if(i != 0){
            appendData(self->byteStream, ",", 1, "", 0);
        }

        appendString(self->byteStream, self->namesOfAddresses[i], strlen(self->namesOfAddresses[i]), ":[", 2);
        appendNumber(self->byteStream, self->addresses[i], ",", 1);

        appendString(self->byteStream, self->typesOfAddresses[i], strlen(self->typesOfAddresses[i]), "]", 1);
    }

    appendData(self->byteStream, ",", 1, "", 0);

    appendString(self->byteStream, KEYWORD_NUMBER_OF_CHANNELS, KEYWORD_NUMBER_OF_CHANNELS_LENGTH, ":", 1);
    appendNumber(self->byteStream, self->maxNumberOfChannels, "", 0);

    appendData(self->byteStream, "}", 1, "", 0);

    self->addressesReady = false;

    return true;
}

static bool packChannel(JsonPackerHandle self, bool commaIsNeeded){

    if(self->channelsReady == false){
        return commaIsNeeded;
    }

    addComma(self->byteStream, commaIsNeeded);
    appendString(self->byteStream, KEYWORD_CL_DATA, KEYWORD_CL_DATA_LENGTH, ":{", 2);

    for(size_t i = 0; i < self->amountOfChannelsToSend; ++i){
        if(self->channelBuffers[i] != NULL){
            char id[MAX_LENGTH_OF_NUMBER];
            size_t len = 0;
#if (ARCH_SIZE_32)
            len = sprintf(id, "%u", (unsigned int) self->channelIds[i]);
#else
            len = sprintf(id, "%llu", (long long unsigned int) self->channelIds[i]);
#endif

            /* Add a , in front of the channel data in case it isn't the first one */
            if(i != 0){
                appendData(self->byteStream, ",", 1, "", 0);
            }
            appendString(self->byteStream, id, len, ":[", 2);
            FloatRingBufferHandle buffer = self->channelBuffers[i];
            size_t amountOfDataToWrite = FloatRingBuffer_getNumberOfUsedData(buffer);
            float data = 0.0f;

            FloatRingBuffer_read(buffer, &data, 1);
            appendFloat(data, self->byteStream);

            for(int j = 1; j < amountOfDataToWrite; ++j){

                self->byteStream->writeByte(self->byteStream, ',');
                FloatRingBuffer_read(buffer, &data, 1);
                appendFloat(data, self->byteStream);
            }
        }
        appendData(self->byteStream, "]", 1, "", 0);
    }

    appendData(self->byteStream, "}", 1, "", 0);

    self->channelsReady = false;

    return true;
}

static bool channelMapIsEmpty(JsonPackerHandle self){

    return (!self->channelsReady && !self->timestampReady && !self->tIncReady &&
            !self->triggerReady && !self->addressesReady);
}

static bool packChannelMap(JsonPackerHandle self){

    if(channelMapIsEmpty(self)){
        return false;
    }

    bool commaIsNeeded = false;

    appendString(self->byteStream, KEYWORD_SC_DATA, KEYWORD_SC_DATA_LENGTH, ":{", 2);

    /* Merge all the pre packed sc data buffers together */
    commaIsNeeded = packChannel(self, commaIsNeeded);
    commaIsNeeded = packTimestamp(self, commaIsNeeded);
    commaIsNeeded = packTimeIncrement(self, commaIsNeeded);
    commaIsNeeded = packTrigger(self, commaIsNeeded);
    packAddressAnnouncement(self, commaIsNeeded);

    appendData(self->byteStream, "}", 1, "", 0);

    return true;
}

static bool packPayloadMap(JsonPackerHandle self){

    appendData(self->byteStream, "{", 1, "", 0);
    appendString(self->byteStream, KEYWORD_PAYLOAD, KEYWORD_PAYLOAD_LENGTH, ":{", 2);

    bool commaIsNeeded = packChannelMap(self);
    commaIsNeeded = packFlowControl(self, commaIsNeeded);

    appendData(self->byteStream, "}", 1, "", 0);
    appendData(self->byteStream, "}", 1, "", 0);

    return commaIsNeeded;
}

static void packData(IPackerHandle packer){
    JsonPackerHandle self = (JsonPackerHandle) packer->handle;

    if(self->dataPendingToBePacked == false){
        return;
    }

    self->byteStream->flush(self->byteStream);
    packPayloadMap(self);
    packer->reset(packer);
}

static void reset(IPackerHandle packer){
    JsonPackerHandle self = (JsonPackerHandle) packer->handle;

    self->addressesReady = false;
    self->channelsReady = false;
    self->numberOfAddressesToAnnounce = 0;
    self->amountOfChannelsToSend = 0;

    for(int i = 0; i < self->maxNumberOfChannels; ++i){
        self->channelBuffers[i] = NULL;
    }

    self->tIncReady = false;
    self->timestampReady = false;
    self->timestamp = NULL;
    self->triggerReady = false;
    self->flowcontrolReady = false;
    self->dataPendingToBePacked = false;

    flushBuffer(self->flowcontrol);
}

static bool flowControlReadyToSend(IPackerHandle packer){
    JsonPackerHandle self = (JsonPackerHandle) packer->handle;

    return self->flowcontrolReady;
}

/******************************************************************************
 Public functions
******************************************************************************/
JsonPackerHandle JsonPacker_create(size_t maxNumberOfChannels, size_t maxAddressesToAnnounce,
                                   IByteStreamHandle byteStream){

    JsonPackerHandle self = (JsonPackerHandle) malloc(sizeof(JsonPackerPrivateData));

    self->channelBuffers = malloc(sizeof(float*) * maxNumberOfChannels);
    self->channelIds = malloc(sizeof(uint32_t) * maxNumberOfChannels);
    self->amountOfChannelsToSend = 0;
    self->maxNumberOfChannels = maxNumberOfChannels;
    self->maxAddressesToAnnounce = maxAddressesToAnnounce;
    self->addresses = malloc(sizeof(ADDRESS_DATA_TYPE) * maxAddressesToAnnounce);
    self->namesOfAddresses = malloc(sizeof(char*) * maxAddressesToAnnounce);
    self->typesOfAddresses = malloc(sizeof(char*) * maxAddressesToAnnounce);

    self->byteStream = byteStream;
    self->dataPendingToBePacked = false;

    self->packer.handle = self;
    self->packer.pack = &packData;
    self->packer.reset = &reset;
    self->packer.prepareChannel = prepareChannel;
    self->packer.prepareFlowControl = &prepareFlowControl;
    self->packer.prepareTimeIncrement = &prepareTimeIncrement;
    self->packer.prepareTimestamp = &prepareTimestamp;
    self->packer.prepareTrigger = &prepareTrigger;
    self->packer.prepareAddressAnnouncement = &prepareAddressAnnouncement;
    self->packer.flowControlReadyToSend = &flowControlReadyToSend;
    self->packer.reset(&self->packer);

    return self;
}

void JsonPacker_destroy(JsonPackerHandle self){

    free(self->addresses);
    self->addresses = NULL;
    free(self->namesOfAddresses);
    self->namesOfAddresses = NULL;
    free(self->typesOfAddresses);
    self->typesOfAddresses = NULL;
    free(self->channelBuffers);
    self->channelBuffers = NULL;
    free(self->channelIds);
    self->channelIds = NULL;

    free(self);
    self = NULL;
}

size_t JsonPacker_calculateBufferSize(size_t maxNumberOfChannels, size_t sizeOfChannels, size_t maxAddressesToAnnounce){


    /* The channel buffer needs enough space to print all data points. This allows for all channels to have numbers whith MAX_LENGTH_OF_NUMBER digits.
     * 20 bytes will be reserved for the over head
     * sizeOfChannels has to be added to allow space for the ,*/
    size_t channelBufferSize =
            ((MAX_LENGTH_OF_NUMBER + 1) * sizeOfChannels + MAX_CONTROL_SIGN_SIZE) * maxNumberOfChannels +
            MAX_CONTROL_SIGN_SIZE;

    /* The timestamp buffer needs enough space to print all data points.
     * Again approximately 20 bytes should be used for the overhead
     * sizeOfChannels has to be added to allow space for the ,*/
    size_t timestampBufferSize = ((MAX_LENGTH_OF_NUMBER + 1) * sizeOfChannels + MAX_CONTROL_SIGN_SIZE);
    size_t announcementBufferSize =
            (MAX_LENGTH_OF_NUMBER + maxAddrNameLength) * maxAddressesToAnnounce + MAX_CONTROL_SIGN_SIZE +
            MAX_LENGTH_OF_NUMBER * 5;
    size_t scopeDataBufferSize =
            announcementBufferSize + timestampBufferSize + channelBufferSize + MAX_CONTROL_SIGN_SIZE \
 + TINC_BUFFER_SIZE + TRIGGER_BUFFER_SIZE;
    size_t payloadBufferSize = scopeDataBufferSize + MAX_CONTROL_SIGN_SIZE + FLOWCONTROL_BUFFER_SIZE;
    size_t outputBufferSize = payloadBufferSize + 30;

    return outputBufferSize;

}

IPackerHandle JsonPacker_getIPacker(JsonPackerHandle self){
    return &self->packer;
}
