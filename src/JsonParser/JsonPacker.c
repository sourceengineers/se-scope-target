/*!****************************************************************************************************************************************
 * @file         JsonPacker.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/JsonParser/JsonPacker.h>
#include <string.h>
#include <stdio.h>
#include <Scope/JsonParser/JsonCommon.h>
#include <Scope/Communication/Keywords.h>
//#include <math.h>

#define FLOWCONTROL_BUFFER_SIZE 30
#define TINC_BUFFER_SIZE 30
#define TRIGGER_BUFFER_SIZE 60

#define MAX_CONTROL_SIGN_SIZE 30

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __JsonPackerPrivateData
{
  size_t maxNumberOfChannels;

  IPacker packer;

  /* Data for the Announce address feature */
  size_t maxAddressesToAnnounce;
  char** namesOfAddresses;
  char** typesOfAddresses;
  gemmi_uint* addresses;
  gemmi_uint numberOfAddressesToAnnounce;
  bool addressesReady;

  IComValidatorHandle validator;

  IByteStreamHandle byteStream;

  /* Channel preparation data */
  bool channelsReady;
  size_t numberOfChannelsToSend;
  gemmi_uint* channelIds;
  IFloatStreamHandle* floatStreams;

  /* Timestamp increment data */
  bool tIncReady;
  gemmi_uint timeIncrement;

  /* Timestamp data */
  bool timestampReady;
  IIntStreamHandle timestamp;

  /* Trigger data */
  bool triggerReady;
  bool isTriggered;
  gemmi_uint channelId;
  gemmi_uint triggerTimestamp;

  /* Flow control data */
  bool flowcontrolReady;
  char flowcontrol[4];

} JsonPackerPrivateData ;

static void appendString(IByteStreamHandle destination, const char* origin, const char* endWith);
static void appendNumber(IByteStreamHandle destination, gemmi_uint origin, const char* endWith);
static void flushBuffer(char* buffer);
static void addComma(IByteStreamHandle destination, bool commaIsNeeded);
static bool channelMapIsEmpty(JsonPackerHandle self);
static void formatCheck(char* formatedCheck, uint8_t* check, size_t checkLength);

static bool packTimeIncrement(JsonPackerHandle self, bool commaIsNeeded);
static void prepareTimestamp(IPackerHandle packer, IIntStreamHandle timestamp);
static bool packTimestamp(JsonPackerHandle self, bool commaIsNeeded);
static bool packTrigger(JsonPackerHandle self, bool commaIsNeeded);
static bool packFlowControl(JsonPackerHandle self, bool commaIsNeeded);
static void prepareAddressAnnouncement(IPackerHandle packer, const char* name, const char* type, const gemmi_uint address);
static bool packAddressAnnouncement(JsonPackerHandle self, bool commaIsNeeded);
static bool packChannel(JsonPackerHandle self, bool commaIsNeeded);
static bool packChannelMap(JsonPackerHandle self);
static bool packPayloadMap(JsonPackerHandle self);
static void packBase(JsonPackerHandle self);

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

inline static void appendNumber(IByteStreamHandle destination, gemmi_uint origin, const char* endWith){

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

static void prepareChannel(IPackerHandle packer, IFloatStreamHandle stream, const gemmi_uint channelId){
  JsonPackerHandle self = (JsonPackerHandle) packer->implementer;

  if(channelId >= self->maxNumberOfChannels){
    return;
  }

  if(self->numberOfChannelsToSend >= self->maxNumberOfChannels){
    return;
  }

  self->channelIds[self->numberOfChannelsToSend] = channelId;
  self->floatStreams[self->numberOfChannelsToSend] = stream;

  self->numberOfChannelsToSend++;

  self->channelsReady = true;
}

static void prepareTimeIncrement(IPackerHandle packer, const gemmi_uint timeIncrement){
  JsonPackerHandle self = (JsonPackerHandle) packer->implementer;

  self->tIncReady = true;
  self->timeIncrement = timeIncrement;

  appendString(self->byteStream, KEYWORD_T_INC, ":");
  appendNumber(self->byteStream, timeIncrement, "");
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
  JsonPackerHandle self = (JsonPackerHandle) packer->implementer;

  self->timestampReady = true;
  self->timestamp = timestamp;
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
  gemmi_uint data[dataLength];

  self->timestamp->read(self->timestamp, data, dataLength);
  for (int i = 0; i < dataLength; ++i) {
    if(i != 0){
      appendData(self->byteStream, ",", "");
    }

    appendNumber(self->byteStream, data[i], "");
  }
  appendData(self->byteStream, "]", "");

  self->timestampReady = false;

  return true;
}

static void prepareTrigger(IPackerHandle packer, const bool isTriggered, const gemmi_uint channelId, const gemmi_uint timestamp){
  JsonPackerHandle self = (JsonPackerHandle) packer->implementer;

  self->triggerReady = true;
  self->isTriggered = isTriggered;
  self->channelId = channelId;
  self->triggerTimestamp = timestamp;
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

  } else {
    appendData(self->byteStream, "false", "}");
  }

  self->triggerReady = false;

  return true;
}

static void prepareFlowControl(IPackerHandle packer, const char* flowControl){
  JsonPackerHandle self = (JsonPackerHandle) packer->implementer;

  /* Flow control data will be "NAK" or "ACK" which is why longer strings will be rejected by default */
  if(strlen(flowControl) > 3){
    return;
  }

  self->flowcontrolReady = true;
  strcpy(self->flowcontrol, flowControl);
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


static void prepareAddressAnnouncement(IPackerHandle packer, const char* name, const char* type, const gemmi_uint address){
  JsonPackerHandle self = (JsonPackerHandle) packer->implementer;

  if(self->numberOfAddressesToAnnounce >= self->maxAddressesToAnnounce){
    return;
  }

  self->addresses[self->numberOfAddressesToAnnounce] = address;
  self->namesOfAddresses[self->numberOfAddressesToAnnounce] = (char*) name;
  self->typesOfAddresses[self->numberOfAddressesToAnnounce] = (char*) type;

  self->numberOfAddressesToAnnounce++;

  self->addressesReady = true;
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

  for (size_t i = 0; i < self->numberOfAddressesToAnnounce; ++i) {

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

  for (size_t i = 0; i < self->numberOfChannelsToSend; ++i) {
    if(self->floatStreams[i] != NULL){
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

      const size_t dataLength = self->floatStreams[i]->length(self->floatStreams[i]);
      float data[dataLength];

      self->floatStreams[i]->read(self->floatStreams[i], data, dataLength);

      for (int j = 0; j < dataLength; ++j) {
        char formatedData[MAX_LENGTH_OF_NUMBER];

        /* add a comma in front of the number, if it is not the first number in the array */
        if(j != 0){
          appendData(self->byteStream, ",", "");
        }
	      snprintf(formatedData, MAX_LENGTH_OF_NUMBER, "%.4g", data[j]);
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

  appendString(self->byteStream, KEYWORD_PAYLOAD, ":{");

  bool commaIsNeeded = packChannelMap(self);
  commaIsNeeded = packFlowControl(self, commaIsNeeded);

  appendData(self->byteStream, "}", "");

  return commaIsNeeded;
}

static void formatCheck(char* formatedCheck, uint8_t* check, size_t checkLength){
  formatedCheck[0] = '\0';

  /* format the check to be in a hex representation */
  for (int i = 0; i < checkLength; ++i) {
    char formatedByte[4];
    sprintf(formatedByte, "%02X", check[i]);

    if(i != checkLength - 1){
      strcat(formatedByte, " ");
    }

    strcat(formatedCheck, formatedByte);
  }
}

static void packBase(JsonPackerHandle self){

  if(self->validator->checkPresentInProtocol(self->validator) == true){
    const size_t lengthCheck = self->validator->getCheckLength(self->validator);
    uint8_t check[lengthCheck];
    char formatedCheck[lengthCheck * 3 + 1];


    /* To be able to generate the checksum, the payload map has to be prepacked */
    packPayloadMap(self);
    size_t payloadBufferSize = self->byteStream->length(self->byteStream);
    uint8_t payloadBuffer[payloadBufferSize];
    self->byteStream->read(self->byteStream, payloadBuffer, payloadBufferSize);

    self->validator->createCheck(self->validator, check, payloadBuffer, payloadBufferSize);

    formatCheck(formatedCheck, check, lengthCheck);


    appendData(self->byteStream,  "{", "");
    appendString(self->byteStream, KEYWORD_TRANSPORT, ":");
    appendString(self->byteStream, formatedCheck, ",");
    appendString(self->byteStream, (char*) payloadBuffer, "");

  } else {

    appendData(self->byteStream,  "{", "");
    appendString(self->byteStream, KEYWORD_TRANSPORT, ":");
    appendData(self->byteStream, "null", ",");

    packPayloadMap(self);
  }

  appendData(self->byteStream, "}", "");
  self->byteStream->writeByte(self->byteStream,(uint8_t) '\0');

}

static void packData(IPackerHandle packer){
  JsonPackerHandle self = (JsonPackerHandle) packer->implementer;

  self->byteStream->flush(self->byteStream);

  packBase(self);

  packer->reset(packer);
}

static void reset(IPackerHandle packer){
  JsonPackerHandle self = (JsonPackerHandle) packer->implementer;

  self->addressesReady = false;
  self->channelsReady = false;

  self->numberOfAddressesToAnnounce = 0;
  self->numberOfChannelsToSend = 0;

  for (int i = 0; i < self->maxNumberOfChannels; ++i) {
    self->floatStreams[i] = NULL;
  }

  self->tIncReady = false;

  self->timestampReady = false;
  self->timestamp = NULL;

  self->triggerReady = false;

  self->flowcontrolReady = false;
  flushBuffer(self->flowcontrol);
}

static IByteStreamHandle getByteStream(IPackerHandle packer){
  JsonPackerHandle self = (JsonPackerHandle) packer->implementer;
  return self->byteStream;
}
/******************************************************************************
 Public functions
******************************************************************************/
JsonPackerHandle JsonPacker_create(size_t maxNumberOfChannels, size_t maxAddressesToAnnounce,
                                   IComValidatorHandle validator,
                                   IByteStreamHandle byteStream){

  JsonPackerHandle self = (JsonPackerHandle) malloc(sizeof(JsonPackerPrivateData));

  self->floatStreams = malloc(sizeof(IFloatStreamHandle) * maxNumberOfChannels);
  self->channelIds = malloc(sizeof(gemmi_uint) * maxNumberOfChannels);
  self->validator = validator;
  self->numberOfChannelsToSend = 0;
  self->maxNumberOfChannels = maxNumberOfChannels;
  self->maxAddressesToAnnounce = maxAddressesToAnnounce;
  self->addresses = malloc(sizeof(gemmi_uint) * maxAddressesToAnnounce);
  self->namesOfAddresses = malloc(sizeof(char*) * maxAddressesToAnnounce);
  self->typesOfAddresses = malloc(sizeof(char*) * maxAddressesToAnnounce);

  self->byteStream = byteStream;

  self->packer.implementer = self;
  self->packer.pack = &packData;
  self->packer.reset = &reset;
  self->packer.prepareChannel = prepareChannel;
  self->packer.prepareFlowControl = &prepareFlowControl;
  self->packer.prepareTimeIncrement = &prepareTimeIncrement;
  self->packer.prepareTimestamp = &prepareTimestamp;
  self->packer.prepareTrigger = &prepareTrigger;
  self->packer.getByteStream = &getByteStream;
  self->packer.prepareAddressAnnouncement = &prepareAddressAnnouncement;

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
  free(self->floatStreams);
  self->floatStreams = NULL;
  free(self->channelIds);
  self->channelIds = NULL;

  free(self);
  self = NULL;
}

size_t JsonPacker_calculateBufferSize(size_t maxNumberOfChannels, size_t maxAddressesToAnnounce,
                                            size_t sizeOfChannels){


  /* The channel buffer needs enough space to print all data points. This allows for all channels to have numbers whith MAX_LENGTH_OF_NUMBER digits.
   * 20 bytes will be reserved for the over head
   * sizeOfChannels has to be added to allow space for the ,*/
  size_t channelBufferSize = ((MAX_LENGTH_OF_NUMBER + 1) * sizeOfChannels + MAX_CONTROL_SIGN_SIZE) * maxNumberOfChannels + MAX_CONTROL_SIGN_SIZE;

  /* The timestamp buffer needs enough space to print all data points.
   * Again approximately 20 bytes should be used for the overhead
   * sizeOfChannels has to be added to allow space for the ,*/
  size_t timestampBufferSize = ((MAX_LENGTH_OF_NUMBER + 1) * sizeOfChannels + MAX_CONTROL_SIGN_SIZE) ;
  size_t announcementBufferSize = (MAX_LENGTH_OF_NUMBER + maxAddrNameLength) * maxAddressesToAnnounce + MAX_CONTROL_SIGN_SIZE + MAX_LENGTH_OF_NUMBER * 5;
  size_t scopeDataBufferSize = announcementBufferSize + timestampBufferSize + channelBufferSize + MAX_CONTROL_SIGN_SIZE \
                              + TINC_BUFFER_SIZE + TRIGGER_BUFFER_SIZE;
  size_t payloadBufferSize = scopeDataBufferSize + MAX_CONTROL_SIGN_SIZE + FLOWCONTROL_BUFFER_SIZE ;
  size_t outputBufferSize = payloadBufferSize + 30;

  return outputBufferSize;

}

IPackerHandle JsonPacker_getIPacker(JsonPackerHandle self){
  return &self->packer;
}
