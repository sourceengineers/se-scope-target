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
#include <stdio.h>
#include <Scope/Serialisation/JsonParser/JsonCommon.h>
#include <Scope/Serialisation/SerializerDefinitions.h>
//#include <math.h>

#define FLOWCONTROL_BUFFER_SIZE 30
#define TINC_BUFFER_SIZE 30
#define TRIGGER_BUFFER_SIZE 60

#define MAX_CONTROL_SIGN_SIZE 30

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __JsonPackerPrivateData{
    size_t maxNumberOfChannels;

    IPacker packer;

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
    float** channelsData;
    size_t* channelSizes;

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

static void appendString(IByteStreamHandle destination, const char* origin, const char* endWith);

static void appendNumber(IByteStreamHandle destination, ADDRESS_DATA_TYPE origin, const char* endWith);

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

/******************************************************************************
 Private functions
******************************************************************************/
inline static void appendString(IByteStreamHandle destination, const char* origin, const char* endWith){
    destination->writeByte(destination, (uint8_t) '\"');
    destination->write(destination, (uint8_t*) origin, strlen(origin));
    destination->writeByte(destination, (uint8_t) '\"');

    if(endWith[0] != '\0'){
        destination->write(destination, (uint8_t*) endWith, strlen(endWith));
    }
}

inline static void appendData(IByteStreamHandle destination, const char* origin, const char* endWith){
    destination->write(destination, (uint8_t*) origin, strlen(origin));
    if(endWith[0] != '\0'){
        destination->write(destination, (uint8_t*) endWith, strlen(endWith));
    }
}

inline static void flushBuffer(char* buffer){
    buffer[0] = '\0';
}

inline static void addComma(IByteStreamHandle destination, bool commaIsNeeded){

    if(commaIsNeeded == true){
        appendData(destination, ",", "");
    }
}

inline static void appendNumber(IByteStreamHandle destination, ADDRESS_DATA_TYPE origin, const char* endWith){

    char number[MAX_LENGTH_OF_NUMBER];

#if (ARCH_SIZE_32)
    sprintf(number, "%u", origin);
#else
    sprintf(number, "%llu", (long long unsigned int) origin);
#endif

    destination->write(destination, (uint8_t*) number, strlen(number));
    if(endWith[0] != '\0'){
        destination->write(destination, (uint8_t*) endWith, strlen(endWith));
    }
}

static void prepareChannel(IPackerHandle packer, float* data, size_t length, const uint32_t channelId){
    JsonPackerHandle self = (JsonPackerHandle) packer->handle;

    if(channelId >= self->maxNumberOfChannels){
        return;
    }

    if(self->amountOfChannelsToSend >= self->maxNumberOfChannels){
        return;
    }

    self->channelIds[self->amountOfChannelsToSend] = channelId;
    self->channelsData[self->amountOfChannelsToSend] = data;
    self->channelSizes[self->amountOfChannelsToSend] = length;

    self->amountOfChannelsToSend++;

    self->channelsReady = true;
    self->dataPendingToBePacked = true;
}

static void prepareTimeIncrement(IPackerHandle packer, const uint32_t timeIncrement){
    JsonPackerHandle self = (JsonPackerHandle) packer->handle;

    self->tIncReady = true;
    self->timeIncrement = timeIncrement;

    appendString(self->byteStream, KEYWORD_T_INC, ":");
    appendNumber(self->byteStream, timeIncrement, "");
    self->dataPendingToBePacked = true;
}

static bool packTimeIncrement(JsonPackerHandle self, bool commaIsNeeded){

    if(self->tIncReady == false){
        return commaIsNeeded;
    }

    addComma(self->byteStream, commaIsNeeded);
    appendString(self->byteStream, KEYWORD_T_INC, ":");
    appendNumber(self->byteStream, self->timeIncrement, "");

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
    appendString(self->byteStream, KEYWORD_T_STMP, ":[");

    const size_t dataLength = self->timestamp->length(self->timestamp);
    uint32_t data[dataLength];

    self->timestamp->read(self->timestamp, data, dataLength);
    for(int i = 0; i < dataLength; ++i){
        if(i != 0){
            appendData(self->byteStream, ",", "");
        }

        appendNumber(self->byteStream, data[i], "");
    }
    appendData(self->byteStream, "]", "");

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
    appendString(self->byteStream, KEYWORD_TGR, ":{");

    appendString(self->byteStream, KEYWORD_TGR_FOUND, ":");

    if(self->isTriggered == true){
        appendData(self->byteStream, "true", ",");

        appendString(self->byteStream, KEYWORD_TGR_CL_DATA_IND, ":");
        appendNumber(self->byteStream, self->triggerTimestamp, ",");

        appendString(self->byteStream, KEYWORD_TGR_CL_ID, ":");
        appendNumber(self->byteStream, self->channelId, "}");

    }else{
        appendData(self->byteStream, "false", "}");
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
    appendString(self->byteStream, KEYWORD_FLOW_CTRL, ":");
    appendString(self->byteStream, self->flowcontrol, "");

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
    appendString(self->byteStream, KEYWORD_ANNOUNCE, ":{");

    for(size_t i = 0; i < self->numberOfAddressesToAnnounce; ++i){

        if(i != 0){
            appendData(self->byteStream, ",", "");
        }

        appendString(self->byteStream, self->namesOfAddresses[i], ":[");
        appendNumber(self->byteStream, self->addresses[i], ",");

        appendString(self->byteStream, self->typesOfAddresses[i], "]");
    }

    appendData(self->byteStream, ",", "");

    appendString(self->byteStream, KEYWORD_NUMBER_OF_CHANNELS, ":");
    appendNumber(self->byteStream, self->maxNumberOfChannels, "");

    appendData(self->byteStream, "}", "");

    self->addressesReady = false;

    return true;
}

static bool packChannel(JsonPackerHandle self, bool commaIsNeeded){

    if(self->channelsReady == false){
        return commaIsNeeded;
    }

    addComma(self->byteStream, commaIsNeeded);
    appendString(self->byteStream, KEYWORD_CL_DATA, ":{");

    for(size_t i = 0; i < self->amountOfChannelsToSend; ++i){
        if(self->channelsData[i] != NULL){
            char id[MAX_LENGTH_OF_NUMBER];
#if (ARCH_SIZE_32)
            sprintf(id, "%u", self->channelIds[i]);
#else
            sprintf(id, "%llu", (long long unsigned int) self->channelIds[i]);
#endif

            /* Add a , in front of the channel data in case it isn't the first one */
            if(i != 0){
                appendData(self->byteStream, ",", "");
            }
            appendString(self->byteStream, id, ":[");

            for(int j = 0; j < self->channelSizes[i]; ++j){
                char formatedData[MAX_LENGTH_OF_NUMBER];

                /* add a comma in front of the number, if it is not the first number in the array */
                if(j != 0){
                    appendData(self->byteStream, ",", "");
                }
                snprintf(formatedData, MAX_LENGTH_OF_NUMBER, "%.4g", self->channelsData[i][j]);
                appendData(self->byteStream, formatedData, "");
            }
        }
        appendData(self->byteStream, "]", "");
    }

    appendData(self->byteStream, "}", "");

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

    appendString(self->byteStream, KEYWORD_SC_DATA, ":{");

    /* Merge all the pre packed sc data buffers together */
    commaIsNeeded = packChannel(self, commaIsNeeded);
    commaIsNeeded = packTimestamp(self, commaIsNeeded);
    commaIsNeeded = packTimeIncrement(self, commaIsNeeded);
    commaIsNeeded = packTrigger(self, commaIsNeeded);
    packAddressAnnouncement(self, commaIsNeeded);

    appendData(self->byteStream, "}", "");

    return true;
}

static bool packPayloadMap(JsonPackerHandle self){

    appendData(self->byteStream, "{", "");
    appendString(self->byteStream, KEYWORD_PAYLOAD, ":{");

    bool commaIsNeeded = packChannelMap(self);
    commaIsNeeded = packFlowControl(self, commaIsNeeded);

    appendData(self->byteStream, "}", "");

    return commaIsNeeded;
}

/*static void formatCheck(char* formatedCheck, uint8_t* check, size_t checkLength){
  formatedCheck[0] = '\0';

  // format the check to be in a hex representation
  for (int i = 0; i < checkLength; ++i) {
    char formatedByte[4];
    sprintf(formatedByte, "%02X", check[i]);

    if(i != checkLength - 1){
      strcat(formatedByte, " ");
    }

    strcat(formatedCheck, formatedByte);
  }
}*/

static void packData(IPackerHandle packer){
    JsonPackerHandle self = (JsonPackerHandle) packer->handle;

    if(self->dataPendingToBePacked == false){
        return;
    }

    self->byteStream->flush(self->byteStream);

    packPayloadMap(self);
    self->byteStream->writeByte(self->byteStream, (uint8_t) '\0');
    packer->reset(packer);
}

static void reset(IPackerHandle packer){
    JsonPackerHandle self = (JsonPackerHandle) packer->handle;

    self->addressesReady = false;
    self->channelsReady = false;
    self->numberOfAddressesToAnnounce = 0;
    self->amountOfChannelsToSend = 0;

    for(int i = 0; i < self->maxNumberOfChannels; ++i){
        self->channelsData[i] = NULL;
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

    self->channelsData = malloc(sizeof(float*) * maxNumberOfChannels);
    self->channelSizes = malloc(sizeof(size_t) * maxNumberOfChannels);
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
    free(self->channelSizes);
    self->channelSizes = NULL;
    free(self->channelsData);
    self->channelsData = NULL;
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
