/*!****************************************************************************************************************************************
 * @file         channel.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        TODO
 *
 *****************************************************************************************************************************************/

#include <scope/channel.h>

/* Define public data */
struct ChannelPrivateStruct
{
  /* _private Data */
  RingBuffer* buffer;
  CL_STATES state;
  float* pollAddress;

  void (*setState)(Channel* self, CL_STATES state);
  CL_STATES (*getState)(Channel* self);
};

Channel* Channel_create(RingBuffer* buffer){

  Channel* self = malloc(sizeof(Channel));
  self->_private = malloc(sizeof(ChannelPrivate));

  /* Set private variables */
  self->_private->state = CL_INIT;
  self->_private->buffer = buffer;
  
  /* Attatch private functions */
  self->_private->setState = &Channel_setState;
  self->_private->getState = &Channel_getState;
  
  /* Attatch public functions */
  self->setPollAddress = &Channel_setPollAddress;
  self->getPollAddress = &Channel_getPollAddress;
  self->setStateRunning = &Channel_setStateRunning;
  self->setStateStopped = &Channel_setStateStopped;
  self->poll = &Channel_poll;
  self->getFloatStream = &Channel_getFloatStream;

  return self;
}

void Channel_destroy(Channel* self){
  RingBuffer_destroy(self->_private->buffer);
  free(self->_private);
  free(self);
}

static void Channel_setPollAddress(Channel* self, float* pollAddress){
  self->_private->pollAddress = pollAddress;
  if(self->_private->getState(self) == CL_INIT){
    self->_private->setState(self, CL_STOPPED);
  }
}

static float* Channel_getPollAddress(Channel* self){
  return self->_private->pollAddress;
}

static void Channel_setState(Channel* self, CL_STATES state){
  self->_private->state = state;
}

static CL_STATES Channel_getState(Channel* self){
  return self->_private->state;
}

static bool Channel_setStateRunning(Channel* self){
  if(self->_private->getState(self) == CL_STOPPED){
    self->_private->setState(self, CL_RUNNING);
    return true;
  } else {
    return false;
  }
}

static bool Channel_setStateStopped(Channel* self){
  if(self->_private->getState(self) == CL_RUNNING){
    self->_private->setState(self, CL_STOPPED);
    return true;
  } else {
    return false;
  }
}

static ssize_t Channel_poll(Channel* self){
  if(self->_private->getState(self) == CL_RUNNING){
    const float polledData = *(self->_private->pollAddress);
    return self->_private->buffer->write(self->_private->buffer, &polledData, 1);  
  } else {
    return -1;
  }
}

static IFloatStream* Channel_getFloatStream(Channel* self){
  return self->_private->buffer->getFloatStream(self->_private->buffer);
}
