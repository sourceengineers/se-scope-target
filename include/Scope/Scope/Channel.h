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

#include <Scope/GeneralPurpose/FloatRingBuffer.h>
#include <Scope/GeneralPurpose/DataTypes.h>

/* Indication for the trigger data */
const int CHANNEL_CURRENT_DATA = 0;
const int CHANNEL_OLD_DATA = 1;

/* Channels States */
typedef enum {CHANNEL_NONE, CHANNEL_INIT, CHANNEL_STOPPED, CHANNEL_RUNNING} CHANNEL_STATES;

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __ChannelPrivateData* ChannelHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the channel */
ChannelHandle Channel_create(FloatRingBufferHandle buffer);

/* Deconstructor: Deletes the instance of the channel */
void Channel_destroy(ChannelHandle self);

/* Sets the address which the channel will poll from
   Sets the state to CHANNEL_RUNNING */
void Channel_setPollAddress(ChannelHandle self, void* pollAddress, const DATA_TYPES pollDataType);

/* Returns the current polling address */
void* Channel_getPollAddress(ChannelHandle self);

/* Sets the channel to CHANNEL_RUNNING
   Returns -1 if the channel wasn't in CHANNEL_STOPPED */
bool Channel_setStateRunning(ChannelHandle self);

/* Sets the channel to CHANNEL_STOPPED
   Returns -1 if the channel wasn't in CHANNEL_RUNNING */
bool Channel_setStateStopped(ChannelHandle self);

/* Returns the state of the channel */
CHANNEL_STATES Channel_getState(ChannelHandle self);

/* Returns true if the channel is running */
bool Channel_isRunning(ChannelHandle self);

/* Polls data and writes it into the ring buffer
   Returns the responce of the buffer->write() functions 
   Will be written data point in a success case and a -1 if something went 
   wrong */
ssize_t Channel_poll(ChannelHandle self);

/* Returns the FloatStream of the buffer, so data can be read */
IFloatStreamHandle Channel_getRingBufferFloatStream(ChannelHandle self);

/* Safes to data points into the triggerData and returns the amount of points 
   written */
IFloatStreamHandle Channel_getTriggerDataStream(ChannelHandle self);

#endif
