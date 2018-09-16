/*!*****************************************************************************
 * @file         channel.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Impelments the channels, which will be attached to the scope
 * 
 ******************************************************************************/
 
#ifndef CHANNEL_H_
#define CHANNEL_H_

#include <scope/ring_buffer.h>

typedef enum CL_STATES_ENUM {CL_INIT, CL_STOPPED, CL_RUNNING} CL_STATES;

/* Defines class */
/* Define private data */
typedef struct ChannelPrivateStruct ChannelPrivate;
/* Define public data */
typedef struct ChannelStruct Channel;

struct ChannelStruct
{
  ChannelPrivate* _private;
  
  void (*setPollAddress)(Channel* self, float* pollAddress);
  float* (*getPollAddress)(Channel* self);
  bool (*setStateRunning)(Channel* self);
  bool (*setStateStopped)(Channel* self);
  ssize_t (*poll)(Channel* self);
  IFloatStream* (*getFloatStream)(Channel* self);
};

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instanze of the channel */
Channel* Channel_create(RingBuffer* buffer);

/* Deconstructor: Deletes the instanze of the channel */
void Channel_destroy(Channel* self);

/* Sets the address which the channel will poll from
   Sets the state to CL_RUNNING */
static void Channel_setPollAddress(Channel* self, float* pollAddress);

/* Returns the current polling address */
static float* Channel_getPollAddress(Channel* self);

/* Sets the channel to CL_RUNNING
   Returns -1 if the channel wasn't in CL_STOPPED */
static bool Channel_setStateRunning(Channel* self);

/* Sets the channel to CL_STOPPED
   Returns -1 if the channel wasn't in CL_RUNNING */
static bool Channel_setStateStopped(Channel* self);

/* Polls data and writes it into the ring buffer
   Returns the responce of the buffer->write() functions 
   Will be written data point in a success case and a -1 if something went 
   wrong */
static ssize_t Channel_poll(Channel* self);

/* Returns the FloatStream of the buffer, so data can be read */
static IFloatStream* Channel_getFloatStream(Channel* self);
/******************************************************************************
 Private Functions 
******************************************************************************/
/* Sets the channel into a new state */
static void Channel_setState(Channel* self, CL_STATES state);

/* Returns the current channel state */
static CL_STATES Channel_getState(Channel* self);
#endif
