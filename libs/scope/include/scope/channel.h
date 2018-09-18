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

static const int CL_CURRENT_DATA = 0;
static const int CL_OLD_DATA = 1;

// fbu: ich würde CHANNEL ausschreiben
typedef enum {CL_INIT, CL_STOPPED, CL_RUNNING} CL_STATES;


// du kannst im header auf das struct verzichten. Es genügt eine forward declaration
// z.b. typedef struct Channel_PrivateData* Channel_Handle;
// das eigentliche Channel_PrivateData kannst du dann im C-File deklarieren
// schau dir am besten nochmal die generierten templates an, dort sollte das richtig sein.

/* Defines class */
/* Define private data */
typedef struct ChannelPrivateStruct ChannelPrivate;
/* Define public data */
typedef struct ChannelStruct Channel;

struct ChannelStruct
{
  ChannelPrivate* _private;

  // ich glaube auf die funktionspointer kannst du hier verzichten. Die braucht es nur im interface, oder wenn du
  // vererbung anwendest
  void (*setPollAddress)(Channel* self, float* pollAddress);
  float* (*getPollAddress)(Channel* self);
  bool (*setStateRunning)(Channel* self);
  bool (*setStateStopped)(Channel* self);
  ssize_t (*poll)(Channel* self);
  IFloatStream* (*getFloatStream)(Channel* self);
  size_t (*getTriggerData)(Channel* self, float* triggerData);
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

/* Safes to data points into the triggerData and returns the amount of points 
   written */
static size_t Channel_getTriggerData(Channel* self, float* triggerData);

/******************************************************************************
 Private Functions 
******************************************************************************/
/* Sets the channel into a new state */
static void Channel_setState(Channel* self, CL_STATES state);

/* Returns the current channel state */
static CL_STATES Channel_getState(Channel* self);

/* Safes the last two polled data points, so the trigger can fetch them */
static void Channel_prepareTriggerData(Channel* self, float triggerData);

#endif
