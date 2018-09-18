/*!*****************************************************************************
 * @file         Channel.h
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

#include <scope/RingBuffer.h>

static const int CHANNEL_CURRENT_DATA = 0;
static const int CHANNEL_OLD_DATA = 1;

// fbu: ich würde CHANNEL ausschreiben
typedef enum {CHANNEL_INIT, CHANNEL_STOPPED, CHANNEL_RUNNING} CHANNEL_STATES;


// du kannst im header auf das struct verzichten. Es genügt eine forward declaration
// z.b. typedef struct Channel_PrivateData* Channel_Handle;
// das eigentliche Channel_PrivateData kannst du dann im C-File deklarieren
// schau dir am besten nochmal die generierten templates an, dort sollte das richtig sein.

/* Defines class */
typedef struct ChannelPrivateStruct* ChannelHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instanze of the channel */
ChannelHandle Channel_create(RingBufferHandle buffer);

/* Deconstructor: Deletes the instanze of the channel */
void Channel_destroy(ChannelHandle self);

/* Sets the address which the channel will poll from
   Sets the state to CHANNEL_RUNNING */
static void Channel_setPollAddress(ChannelHandle self, float* pollAddress);

/* Returns the current polling address */
static float* Channel_getPollAddress(ChannelHandle self);

/* Sets the channel to CHANNEL_RUNNING
   Returns -1 if the channel wasn't in CHANNEL_STOPPED */
static bool Channel_setStateRunning(ChannelHandle self);

/* Sets the channel to CHANNEL_STOPPED
   Returns -1 if the channel wasn't in CHANNEL_RUNNING */
static bool Channel_setStateStopped(ChannelHandle self);

/* Polls data and writes it into the ring buffer
   Returns the responce of the buffer->write() functions 
   Will be written data point in a success case and a -1 if something went 
   wrong */
static ssize_t Channel_poll(ChannelHandle self);

/* Returns the FloatStream of the buffer, so data can be read */
static IFloatStreamHandle Channel_getFloatStream(ChannelHandle self);

/* Safes to data points into the triggerData and returns the amount of points 
   written */
static size_t Channel_getTriggerData(ChannelHandle self, float* triggerData);

#endif
