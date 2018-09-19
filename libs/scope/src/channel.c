/*!****************************************************************************************************************************************
 * @file         Channel.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Channel.h>

// hier typedef vorne dran, damit das ganze mit der deklaration im header geht
/* Define public data */
struct __ChannelPrivateData
{
  /* _private Data */
  RingBufferHandle buffer;
  CHANNEL_STATES state;
  float* pollAddress;
  float triggerData[2];
} ChannelPrivateData ;

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

ChannelHandle Channel_create(RingBufferHandle buffer){

   // wenn du auf die funktionspointer in der klasse verzichtest kannst du hier einfach ein private erzeugen. das reicht dann schon
   // von aussen ist ja nur der pointer auf das private als "handle" bekannt, da kann nichts schief gehen.
  ChannelHandle self = malloc(sizeof(ChannelPrivateData));

  /* Set private variables */
  self->state = CHANNEL_INIT;
  self->buffer = buffer;
  self->triggerData[CHANNEL_CURRENT_DATA] = 0.0f;
  self->triggerData[CHANNEL_OLD_DATA] = 0.0f;

  return self;
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

size_t Channel_getTriggerData(ChannelHandle self, float* triggerData){
    triggerData[CHANNEL_CURRENT_DATA] = self->triggerData[CHANNEL_CURRENT_DATA];
    triggerData[CHANNEL_OLD_DATA] = self->triggerData[CHANNEL_OLD_DATA];
    return 2;
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

IFloatStreamHandle Channel_getFloatStream(ChannelHandle self){
  return RingBuffer_getFloatStream(self->buffer);
}
