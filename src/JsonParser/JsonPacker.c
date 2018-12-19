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
#include <Scope/GeneralPurpose/Memory.h>
#include <Scope/Communication/Keywords.h>

#if (ARCH_SIZE_32)
const size_t maxLengthOfNumber = 11;
#else
const size_t maxLengthOfNumber = 21;
#endif

const size_t flowControlBufferSize = 30;
const size_t tincBufferSize = 30;
const size_t triggerBufferSize = 60;

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __JsonPackerPrivateData
{
  size_t maxNumberOfChannels;

  IPacker iPacker;

  char* channelBuffer;
  char* timestampBuffer;
  char* announcementBuffer;
  char flowControlBuffer[flowControlBufferSize];
  char tincBuffer[tincBufferSize];
  char triggerBuffer[triggerBufferSize];

  size_t scopeDataBufferSize;
  size_t payloadBufferSize;
  size_t outputBufferSize;

  /* Data for the Announce address feature */
  size_t maxAddressesToAnnounce;
  char** namesOfAddresses;
  char** typesOfAddresses;
  gemmi_uint* addresses;
  gemmi_uint numberOfAddressesToAnnounce;

  IComValidatorHandle validator;

  IByteStreamHandle byteStream;

  /* Channel preparation data */
  bool channelsArePrepared;
  size_t numberOfChannelsToSend;
  gemmi_uint* channelIds;
  IFloatStreamHandle* floatStreams;
} JsonPackerPrivateData ;

static bool bufferIsEmpty(const char* buffer);
static void appendString(char* destination, const char* origin, const char* endWith);
static void appendNumber(char* destination, gemmi_uint origin, const char* endWith);
static void flushBuffer(char* buffer);

/******************************************************************************
 Private functions
******************************************************************************/

inline static void appendString(char* destination, const char* origin, const char* endWith){
  strcat(destination, "\"");
  strcat(destination, origin);
  strcat(destination, "\"");
  strcat(destination, endWith);
}

inline static void flushBuffer(char* buffer){
  buffer[0] = '\0';
}

inline static void appendNumber(char* destination, gemmi_uint origin, const char* endWith){

  char number[maxLengthOfNumber];

#if (ARCH_SIZE_32)
  sprintf(number, "%d", origin);
#else
  sprintf(number, "%llu", origin);
#endif

  strcat(destination, number);
  strcat(destination, endWith);
}

static void prepareChannel(IPackerHandle iPacker, IFloatStreamHandle stream, const gemmi_uint channelId){
  JsonPackerHandle self = (JsonPackerHandle) iPacker->implementer;

  if(channelId >= self->maxNumberOfChannels){
    return;
  }

  if(self->numberOfChannelsToSend >= self->maxNumberOfChannels){
    return;
  }

  self->channelIds[self->numberOfChannelsToSend] = channelId;
  self->floatStreams[self->numberOfChannelsToSend] = stream;

  self->numberOfChannelsToSend++;

  if(self->channelsArePrepared == true){
    return;
  }

  self->channelsArePrepared = true;
}

static void prepareTimeIncrement(IPackerHandle iPacker, const gemmi_uint timeIncrement){
  JsonPackerHandle self = (JsonPackerHandle) iPacker->implementer;

  flushBuffer(self->tincBuffer);
  appendString(self->tincBuffer, KEYWORD_T_INC, ":");
  appendNumber(self->tincBuffer, timeIncrement, "");
}

static void prepareTimestamp(IPackerHandle iPacker, IIntStreamHandle timestamp){
  JsonPackerHandle self = (JsonPackerHandle) iPacker->implementer;

  flushBuffer(self->timestampBuffer);
  appendString(self->timestampBuffer, KEYWORD_T_STMP, ":[");

  const size_t dataLength = timestamp->length(timestamp);
  gemmi_uint data[dataLength];

  timestamp->read(timestamp, data, dataLength);
  for (int i = 0; i < dataLength; ++i) {
    if(i != 0){
      strcat(self->timestampBuffer, ",");
    }

    appendNumber(self->timestampBuffer, data[i], "");
  }
  strcat(self->timestampBuffer, "]");

}

static void prepareTrigger(IPackerHandle iPacker, const bool isTriggered, const gemmi_uint channelId, const gemmi_uint timestamp){
  JsonPackerHandle self = (JsonPackerHandle) iPacker->implementer;

  flushBuffer(self->triggerBuffer);

  /* Prepare map */
  appendString(self->triggerBuffer, KEYWORD_TGR, ":{");

  appendString(self->triggerBuffer, KEYWORD_TGR_FOUND, ":");

  if(isTriggered == true){
    appendString(self->triggerBuffer, "true", ",");
  } else {
    appendString(self->triggerBuffer, "false", "}");
    return;
  }

  appendString(self->triggerBuffer, KEYWORD_TGR_CL_DATA_IND, ":");
  appendNumber(self->triggerBuffer, timestamp, ",");

  appendString(self->triggerBuffer, KEYWORD_TGR_CL_ID, ":");
  appendNumber(self->triggerBuffer, channelId, "}");

}

static void prepareFlowControl(IPackerHandle iPacker, const char* flowControl){
  JsonPackerHandle self = (JsonPackerHandle) iPacker->implementer;

  /* Flow control data will be "NAK" or "ACK" which is why longer strings will be rejected by default */
  if(strlen(flowControl) > 3){
    return;
  }

  flushBuffer(self->flowControlBuffer);
  appendString(self->flowControlBuffer, KEYWORD_FLOW_CTRL, ":");
  appendString(self->flowControlBuffer, flowControl, "");
}

static void prepareAddressAnnouncement(IPackerHandle iPacker, const char* name, const char* type, const gemmi_uint address){
  JsonPackerHandle self = (JsonPackerHandle) iPacker->implementer;

  if(self->numberOfAddressesToAnnounce >= self->maxAddressesToAnnounce){
    return;
  }

  self->addresses[self->numberOfAddressesToAnnounce] = address;
  self->namesOfAddresses[self->numberOfAddressesToAnnounce] = (char*) name;
  self->typesOfAddresses[self->numberOfAddressesToAnnounce] = (char*) type;

  self->numberOfAddressesToAnnounce++;

}

void packAddressAnnouncement(JsonPackerHandle self){

  if(self->numberOfAddressesToAnnounce == 0){
    return;
  }

  flushBuffer(self->announcementBuffer);

  appendString(self->announcementBuffer, KEYWORD_ANNOUNCE, ":{");

  for (size_t i = 0; i < self->numberOfAddressesToAnnounce; ++i) {

    if(i != 0){
      strcat(self->announcementBuffer, ",");
    }

    appendString(self->announcementBuffer, self->namesOfAddresses[i], ":[");
    appendNumber(self->announcementBuffer, self->addresses[i], ",");

    appendString(self->announcementBuffer, self->typesOfAddresses[i], "");
    strcat(self->announcementBuffer, "]");
  }

  strcat(self->announcementBuffer, ",");

  appendString(self->announcementBuffer, KEYWORD_NUMBER_OF_CHANNELS, ":");
  appendNumber(self->announcementBuffer, self->maxNumberOfChannels, "");

  strcat(self->announcementBuffer, "}");
}

static void packChannel(JsonPackerHandle self){

  if(self->channelsArePrepared == false){
    return;
  }

  flushBuffer(self->channelBuffer);
  appendString(self->channelBuffer, KEYWORD_CL_DATA, ":{");

  for (size_t i = 0; i < self->numberOfChannelsToSend; ++i) {

    char id[maxLengthOfNumber];
    sprintf(id, "%u", self->channelIds[i]);

    /* Add a , in front of the channel data in case it isn't the first one */
    if(i != 0){
      strcat(self->channelBuffer, ",");
    }
    appendString(self->channelBuffer, id, ":[");

    const size_t dataLength = self->floatStreams[i]->length(self->floatStreams[i]);
    float data[dataLength];

    self->floatStreams[i]->read(self->floatStreams[i], data, dataLength);

    for (int j = 0; j < dataLength; ++j) {
      char formatedData[maxLengthOfNumber];

      /* add a comma in front of the number, if it is not the first number in the array */
      if(j != 0){
        strcat(self->channelBuffer, ",");
      }

      sprintf(formatedData, "%f", data[j]);
      strcat(self->channelBuffer, formatedData);
    }
    strcat(self->channelBuffer, "]");
  }

  strcat(self->channelBuffer, "}");

  self->channelsArePrepared = false;
}

static void constructBase(JsonPackerHandle self, char* outputBuffer, size_t maxLengthOfBuffer, char* payloadDataBuffer){

  if((strlen(payloadDataBuffer) >= maxLengthOfBuffer) || (strlen(payloadDataBuffer) == 0)){
    return;
  }

  flushBuffer(outputBuffer);
  strcat(outputBuffer, "{");
  appendString(outputBuffer, KEYWORD_TRANSPORT, ":");

  if(self->validator->checkPresentInProtocol(self->validator) == true){
    const size_t lengthCheck = self->validator->getCheckLength(self->validator);
    uint8_t check[lengthCheck];
    char formatedCheck[lengthCheck * 3 + 1];
    formatedCheck[0] = '\0';

    self->validator->createCheck(self->validator,check, (const uint8_t*) payloadDataBuffer, strlen(payloadDataBuffer));

    /* format the check to be in a hex representation */
    for (int i = 0; i < lengthCheck; ++i) {
      char formatedByte[4];
      sprintf(formatedByte, "%02X", check[i]);

      if(i != lengthCheck - 1){
        strcat(formatedByte, " ");
      }

      strcat(formatedCheck, formatedByte);
    }
    appendString(outputBuffer, formatedCheck, "");
  } else {
    strcat(outputBuffer, "null");
  }

  if(!bufferIsEmpty(payloadDataBuffer)) {
    strcat(outputBuffer, ",");
  }
  strcat(outputBuffer, payloadDataBuffer);
  strcat(outputBuffer, "}");

}

static void constructPayloadMap(JsonPackerHandle self, char* payloadDataBuffer, size_t maxLengthOfBuffer, char* scDataBuffer){

  size_t expectedLength = strlen(scDataBuffer) + strlen(self->flowControlBuffer);

  if((expectedLength >= maxLengthOfBuffer) || (expectedLength == 0)){
    return;
  }

  flushBuffer(payloadDataBuffer);
  appendString(payloadDataBuffer, KEYWORD_PAYLOAD, ":{");

  strcat(payloadDataBuffer, scDataBuffer);

  if(!bufferIsEmpty(scDataBuffer)){
    strcat(payloadDataBuffer, ",");
  }
  strcat(payloadDataBuffer, self->flowControlBuffer);

  strcat(payloadDataBuffer, "}");
}

inline static bool bufferIsEmpty(const char* buffer){
  return buffer[0] == '\0';
}

static void constructScDataMap(JsonPackerHandle self, char* scDataBuffer, size_t maxLengthOfBuffer){

  size_t expectedLength = strlen(self->announcementBuffer) + strlen(self->triggerBuffer) + strlen(self->timestampBuffer) \
                          + strlen(self->channelBuffer) + strlen(self->tincBuffer);

  if((expectedLength >= maxLengthOfBuffer) || (expectedLength == 0)){
    return;
  }

  flushBuffer(scDataBuffer);
  appendString(scDataBuffer, KEYWORD_SC_DATA, ":{");

  /* Merge all the pre packed sc data buffers together */
  strcat(scDataBuffer, self->channelBuffer);

  /* If more than one object is prepared, the objects have to be separated by , */
  if(!bufferIsEmpty(self->channelBuffer)){
    strcat(scDataBuffer, ",");
  }
  strcat(scDataBuffer, self->timestampBuffer);

  if(!bufferIsEmpty(self->channelBuffer) || !bufferIsEmpty(self->timestampBuffer)){
    strcat(scDataBuffer, ",");
  }
  strcat(scDataBuffer, self->tincBuffer);

  if(!bufferIsEmpty(self->channelBuffer) || !bufferIsEmpty(self->timestampBuffer) || \
      !bufferIsEmpty(self->tincBuffer)){
    strcat(scDataBuffer, ",");
  }
  strcat(scDataBuffer, self->triggerBuffer);

  if(!bufferIsEmpty(self->channelBuffer) || !bufferIsEmpty(self->timestampBuffer) || \
      !bufferIsEmpty(self->tincBuffer) || !bufferIsEmpty(self->triggerBuffer)){
    strcat(scDataBuffer, ",");
  }
  strcat(scDataBuffer, self->announcementBuffer);

  strcat(scDataBuffer, "}");
}

static void packData(IPackerHandle iPacker){
  JsonPackerHandle self = (JsonPackerHandle) iPacker->implementer;

  char scDataBuffer[self->scopeDataBufferSize];
  char payloadBuffer[self->payloadBufferSize];
  char outputBuffer[self->outputBufferSize];

  packChannel(self);
  packAddressAnnouncement(self);

  constructScDataMap(self, scDataBuffer, self->scopeDataBufferSize);
  constructPayloadMap(self, payloadBuffer, self->payloadBufferSize, scDataBuffer);
  constructBase(self, outputBuffer, self->outputBufferSize, payloadBuffer);

  self->byteStream->flush(self->byteStream);
  /* One additional byte has to be copied to make sure that the string terminator is copied too */
  self->byteStream->write(self->byteStream, (const uint8_t*) outputBuffer, strlen(outputBuffer) + 1);

  iPacker->reset(iPacker);

}

static void reset(IPackerHandle iPacker){
  JsonPackerHandle self = (JsonPackerHandle) iPacker->implementer;

  self->channelsArePrepared = false;
  self->numberOfChannelsToSend = 0;
  self->numberOfAddressesToAnnounce = 0;

  self->channelBuffer[0] = '\0';
  self->timestampBuffer[0] = '\0';
  self->announcementBuffer[0] = '\0';
  self->flowControlBuffer[0] = '\0';
  self->tincBuffer[0] = '\0';
  self->triggerBuffer[0] = '\0';
}

static IByteStreamHandle getByteStream(IPackerHandle iPacker){
  JsonPackerHandle self = (JsonPackerHandle) iPacker->implementer;
  return self->byteStream;
}
/******************************************************************************
 Public functions
******************************************************************************/
JsonPackerHandle JsonPacker_create(OutputBufferSizes sizes,
                                   IComValidatorHandle validator,
                                   IByteStreamHandle byteStream){

  JsonPackerHandle self = (JsonPackerHandle) malloc(sizeof(JsonPackerPrivateData));

  self->floatStreams = malloc(sizeof(IFloatStreamHandle) * sizes.maxNumberOfChannels);
  self->channelIds = malloc(sizeof(gemmi_uint) * sizes.maxNumberOfChannels);
  self->validator = validator;
  self->numberOfChannelsToSend = 0;
  self->maxNumberOfChannels = sizes.maxNumberOfChannels;
  self->maxAddressesToAnnounce = sizes.maxAddressesToAnnounce;
  self->addresses = malloc(sizeof(gemmi_uint) * sizes.maxAddressesToAnnounce);
  self->namesOfAddresses = malloc(sizeof(char*) * sizes.maxAddressesToAnnounce);
  self->typesOfAddresses = malloc(sizeof(char*) * sizes.maxAddressesToAnnounce);

  self->channelBuffer = malloc(sizeof(char) * sizes.channelBufferSize);
  self->timestampBuffer = malloc(sizeof(char) * sizes.timestampBufferSize);
  self->announcementBuffer = malloc(sizeof(char) * sizes.announcementBufferSize);

  self->scopeDataBufferSize = sizes.scopeDataBufferSize;
  self->payloadBufferSize = sizes.payloadBufferSize;
  self->outputBufferSize = sizes.outputBufferSize;

  self->byteStream = byteStream;

  self->iPacker.implementer = self;
  self->iPacker.pack = &packData;
  self->iPacker.reset = &reset;
  self->iPacker.prepareChannel = prepareChannel;
  self->iPacker.prepareFlowControl = &prepareFlowControl;
  self->iPacker.prepareTimeIncrement = &prepareTimeIncrement;
  self->iPacker.prepareTimestamp = &prepareTimestamp;
  self->iPacker.prepareTrigger = &prepareTrigger;
  self->iPacker.getByteStream = &getByteStream;
  self->iPacker.prepareAddressAnnouncement = &prepareAddressAnnouncement;

  self->iPacker.reset(&self->iPacker);

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
  free(self->channelBuffer);
  self->channelBuffer = NULL;
  free(self->timestampBuffer);
  self->timestampBuffer = NULL;
  free(self->announcementBuffer);
  self->announcementBuffer = NULL;

  free(self);
  self = NULL;
}

OutputBufferSizes JsonPacker_calculateBufferSizes(size_t maxNumberOfChannels, size_t maxAddressesToAnnounce,
                                            size_t sizeOfChannels){

  OutputBufferSizes sizes;

  sizes.maxAddressesToAnnounce = maxAddressesToAnnounce;
  sizes.maxNumberOfChannels = maxNumberOfChannels;
  sizes.sizeOfChannels = sizeOfChannels;

  /* The channel buffer needs enough space to print all data points. This allows for all channels to have numbers whith maxLengthOfNumber digits.
   * 20 bytes will be reserved for the over head
   * sizeOfChannels has to be added to allow space for the ,*/
  sizes.channelBufferSize = sizeOfChannels * maxNumberOfChannels * maxLengthOfNumber + 20 + sizeOfChannels * 5;

  /* The timestamp buffer needs enough space to print all data points.
   * Again approximately 20 bytes should be used for the overhead
   * sizeOfChannels has to be added to allow space for the ,*/
  sizes.timestampBufferSize = sizeOfChannels * maxLengthOfNumber + sizeOfChannels + 20;
  sizes.announcementBufferSize = (maxLengthOfNumber + maxAddrNameLength) * maxAddressesToAnnounce + 20 + maxLengthOfNumber * 5;
  sizes.scopeDataBufferSize = sizes.announcementBufferSize + sizes.timestampBufferSize + sizes.channelBufferSize + 20 \
                              + tincBufferSize + triggerBufferSize;
  sizes.payloadBufferSize = sizes.scopeDataBufferSize + 30 + flowControlBufferSize ;
  sizes.outputBufferSize = sizes.payloadBufferSize + 30;

  return sizes;

}

IPackerHandle JsonPacker_getIPacker(JsonPackerHandle self){
  return &self->iPacker;
}
