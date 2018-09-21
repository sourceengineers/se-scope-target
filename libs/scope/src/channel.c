/*!****************************************************************************************************************************************
 * @file         Channel.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Channel.h>
#include <GeneralPurpose/Types.h>

/* Define public data */
typedef struct __ChannelPrivateData
{
  RingBufferHandle buffer;
  IFloatStream triggerDataStream;
  CHANNEL_STATES state;
  
  DATA_TYPES pollDataType;
  void* pollAddress;
  
  float triggerData[2];
  float* floatStream;
} ChannelPrivateData ;

/* Functions for the IFloatStream interface */
static size_t streamGetSize(IFloatStreamHandle iFloatStream){
  return 2;
}

static void streamOpen(IFloatStreamHandle iFloatStream, float* floatStream){
  ChannelHandle self = (ChannelHandle)iFloatStream->implementer;
  if (floatStream != NULL) {
    self->floatStream = floatStream;
  }
}

static void streamClose(IFloatStreamHandle iFloatStream){
  ChannelHandle self = (ChannelHandle)iFloatStream->implementer;
  self->floatStream = NULL;
}

static size_t streamGetData(IFloatStreamHandle iFloatStream){
  ChannelHandle self = (ChannelHandle)iFloatStream->implementer;
  self->floatStream[CHANNEL_CURRENT_DATA] = self->triggerData[CHANNEL_CURRENT_DATA];
  self->floatStream[CHANNEL_OLD_DATA] = self->triggerData[CHANNEL_OLD_DATA];
  return 2;
}

// Private functions
static float castDataToFloat(ChannelHandle self){
  
  uint32_t transportData32;
  uint64_t transportData64;

  float data;
  
  switch (self->pollDataType) {
    case UINT8:
      transportData32 = *((uint32_t*)self->pollAddress);
      data = ((uint8_t)*((uint8_t*)&transportData32));
      break;
    case UINT16:
      transportData32 = *((uint32_t*)self->pollAddress);
      data = ((uint16_t)*((uint16_t*)&transportData32));
      break;
    case UINT32:
      transportData32 = *((uint32_t*)self->pollAddress);
      data = ((uint32_t)*((uint32_t*)&transportData32));
      break;
    case FLOAT:
      transportData32 = *((uint32_t*)self->pollAddress);
      data = ((float)*((float*)&transportData32));
      break;
    case DOUBLE:
      transportData64 = *((uint64_t*)self->pollAddress);
      data = ((double)*((double*)&transportData64));
      break;
    case UINT64:
      transportData64 = *((uint64_t*)self->pollAddress);
      data = ((uint64_t)*((uint64_t*)&transportData64));
      break;
    default:
      transportData32 = *((uint32_t*)self->pollAddress);
      data = ((float)*((float*)&transportData32));
      break;
  }
  return data;  
}

static void prepareTriggerData(ChannelHandle self, float triggerData){
  self->triggerData[CHANNEL_OLD_DATA] = self->triggerData[CHANNEL_CURRENT_DATA];
  self->triggerData[CHANNEL_CURRENT_DATA] = triggerData;
}

static void setState(ChannelHandle self, CHANNEL_STATES state){
  self->state = state;
}

static CHANNEL_STATES getState(ChannelHandle self){
  return self->state;
}

/* Public functions */
ChannelHandle Channel_create(RingBufferHandle buffer){

  ChannelHandle self = malloc(sizeof(ChannelPrivateData));

  /* Set private variables */
  self->state = CHANNEL_INIT;
  self->buffer = buffer;
  self->triggerData[CHANNEL_CURRENT_DATA] = 0.0f;
  self->triggerData[CHANNEL_OLD_DATA] = 0.0f;

  /* Set interface functions */
  self->triggerDataStream.implementer = self;
  self->triggerDataStream.getSize = &streamGetSize;
  self->triggerDataStream.getStream = &streamGetData;
  self->triggerDataStream.open = &streamOpen;
  self->triggerDataStream.close = &streamClose;

  return self;
}

void Channel_destroy(ChannelHandle self){
  RingBuffer_destroy(self->buffer);
  free(self);
}

void Channel_setPollAddress(ChannelHandle self, void* pollAddress, DATA_TYPES pollDataType){
  self->pollAddress = pollAddress;
  self->pollDataType = pollDataType;
  if(getState(self) == CHANNEL_INIT){
    setState(self, CHANNEL_STOPPED);
  }
}

void* Channel_getPollAddress(ChannelHandle self){
  return self->pollAddress;
}

bool Channel_setStateRunning(ChannelHandle self){
  if(getState(self) == CHANNEL_STOPPED){
    setState(self, CHANNEL_RUNNING);
    return true;
  } else {
    return false;
  }
}

bool Channel_setStateStopped(ChannelHandle self){
  if(getState(self) == CHANNEL_RUNNING){
    setState(self, CHANNEL_STOPPED);
    return true;
  } else {
    return false;
  }
}

IFloatStreamHandle Channel_getTriggerDataStream(ChannelHandle self){
  return &self->triggerDataStream;
}

ssize_t Channel_poll(ChannelHandle self){
  if(getState(self) == CHANNEL_RUNNING){
    const float polledData = castDataToFloat(self);
    prepareTriggerData(self, polledData);
    
    return RingBuffer_write(self->buffer, &polledData, 1);  
  } else {
    return -1;
  }
}

IFloatStreamHandle Channel_getRingBufferFloatStream(ChannelHandle self){
  return RingBuffer_getFloatStream(self->buffer);
}
