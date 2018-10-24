/*!****************************************************************************************************************************************
 * @file         Channel.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Channel.h>
#include <Scope/GeneralPurpose/DataTypes.h>
#include <Scope/GeneralPurpose/FloatStream.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __ChannelPrivateData
{
  FloatRingBufferHandle buffer;
  IFloatStreamHandle stream;
  CHANNEL_STATES state;

  DATA_TYPES pollDataType;
  void* pollAddress;

  float oldTriggerData;
} ChannelPrivateData ;

/* Casts the data of the address to float */
static float castDataToFloat(ChannelHandle self);

/* Pushes the latest data point into the trigger stream and copies the last one
 * further back */
static void prepareTriggerData(ChannelHandle self, float triggerData);

/* Sets a new state */
static void setState(ChannelHandle self, CHANNEL_STATES state);

/******************************************************************************
 Private functions
******************************************************************************/
static float castDataToFloat(ChannelHandle self){

  uint32_t transportData32;

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
      transportData32 = *((uint32_t *)self->pollAddress);
      data = ((float)*((float*)&transportData32));
      break;
/*    case DOUBLE:
      transportData32 = *((uint32_t*)self->pollAddress);
      data = ((double)*((double*)&transportData32));
      break;
    case UINT64:
      transportData32 = *((uint32_t*)self->pollAddress);
      data = ((uint64_t)*((uint64_t*)&transportData32));
      break;*/
    default:
      transportData32 = *((uint32_t*)self->pollAddress);
      data = ((float)*((float*)&transportData32));
      break;
  }
  return data;
}

static void prepareTriggerData(ChannelHandle self, float triggerData){

  self->stream->flush(self->stream);

  self->stream->writeData(self->stream, self->oldTriggerData);
  self->stream->writeData(self->stream, triggerData);

  self->oldTriggerData = triggerData;
}

static void setState(ChannelHandle self, CHANNEL_STATES state){
  self->state = state;
}

/******************************************************************************
 Public functions
******************************************************************************/
ChannelHandle Channel_create(FloatRingBufferHandle buffer){

  ChannelHandle self = malloc(sizeof(ChannelPrivateData));
  self->stream = FloatStream_getFloatStream(FloatStream_create(4));

  /* Set private variables */
  self->state = CHANNEL_INIT;
  self->buffer = buffer;
  self->oldTriggerData = 0.0f;

  return self;
}

void Channel_destroy(ChannelHandle self){
  FloatRingBuffer_destroy(self->buffer);
  self->buffer = NULL;
  free(self);
  self = NULL;
}

bool Channel_isRunning(ChannelHandle self){
  if(self->state == CHANNEL_RUNNING){
    return true;
  }
  return false;
}

void Channel_setPollAddress(ChannelHandle self, void* pollAddress, const DATA_TYPES pollDataType){
  self->pollAddress = pollAddress;
  self->pollDataType = pollDataType;
  if(Channel_getState(self) == CHANNEL_INIT){
    setState(self, CHANNEL_STOPPED);
  }
}

void* Channel_getPollAddress(ChannelHandle self){
  return self->pollAddress;
}

bool Channel_setStateRunning(ChannelHandle self){
  if(Channel_getState(self) == CHANNEL_STOPPED){
    setState(self, CHANNEL_RUNNING);
    return true;
  } else {
    return false;
  }
}

bool Channel_setStateStopped(ChannelHandle self){
  if(Channel_getState(self) == CHANNEL_RUNNING){
    setState(self, CHANNEL_STOPPED);
    return true;
  } else {
    return false;
  }
}

CHANNEL_STATES Channel_getState(ChannelHandle self){
  return self->state;
}

IFloatStreamHandle Channel_getTriggerDataStream(ChannelHandle self){
  return self->stream;
}

ssize_t Channel_poll(ChannelHandle self){
  if(Channel_getState(self) == CHANNEL_RUNNING){
    const float polledData = castDataToFloat(self);
    prepareTriggerData(self, polledData);
    
    return FloatRingBuffer_write(self->buffer, &polledData, 1);
  } else {
    return -1;
  }
}

IFloatStreamHandle Channel_getRingBufferFloatStream(ChannelHandle self){
  return FloatRingBuffer_getFloatStream(self->buffer);
}
