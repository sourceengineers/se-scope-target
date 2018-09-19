/*!****************************************************************************************************************************************
 * @file         Channel.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Channel.h>

/* Define public data */
typedef struct __ChannelPrivateData
{
  RingBufferHandle buffer;
  IFloatStream triggerDataStream;
  CHANNEL_STATES state;
  float* pollAddress;
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

void Channel_destroy(ChannelHandle self){
  RingBuffer_destroy(self->buffer);
  free(self);
}

void Channel_setPollAddress(ChannelHandle self, float* pollAddress){
  self->pollAddress = pollAddress;
  if(getState(self) == CHANNEL_INIT){
    setState(self, CHANNEL_STOPPED);
  }
}

float* Channel_getPollAddress(ChannelHandle self){
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
    const float polledData = *(self->pollAddress);
    prepareTriggerData(self, polledData);
    
    return RingBuffer_write(self->buffer, &polledData, 1);  
  } else {
    return -1;
  }
}

IFloatStreamHandle Channel_getRingBufferFloatStream(ChannelHandle self){
  return RingBuffer_getFloatStream(self->buffer);
}
