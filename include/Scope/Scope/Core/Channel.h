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
#include <Scope/Core/ScopeTypes.h>

/* Indication for the trigger data */
static const int CHANNEL_CURRENT_DATA = 0;
static const int CHANNEL_OLD_DATA = 1;

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __ChannelPrivateData* ChannelHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the channel */
ChannelHandle Channel_create(size_t capacity);

/* Deconstructor: Deletes the instance of the channel */
void Channel_destroy(ChannelHandle self);

/* Return the maximum capacity of the channel */
size_t Channel_getCapacity(ChannelHandle self);

/* Sets the address which the channel will poll from
   Sets the state to CHANNEL_RUNNING */
void Channel_setPollAddress(ChannelHandle self, void* pollAddress, DATA_TYPES pollDataType);

/* Returns the current polling address */
void* Channel_getPollAddress(ChannelHandle self);

/* Sets the channel to CHANNEL_RUNNING
   Returns -1 if the channel wasn't in CHANNEL_STOPPED */
bool Channel_setStateRunning(ChannelHandle self);

/* Sets the channel to CHANNEL_STOPPED
   Returns -1 if the channel wasn't in CHANNEL_RUNNING */
bool Channel_setStateStopped(ChannelHandle self);

/* Returns true if the channel is running */
bool Channel_isRunning(ChannelHandle self);

/* Polls data and writes it into the ring buffer
   Returns the responce of the buffer->write() functions 
   Will be written data point in a success case and a -1 if something went 
   wrong */
void Channel_poll(ChannelHandle self);

/* Safes to data points into the triggerData and returns the amount of points
   written */
IFloatStreamHandle Channel_getTriggerDataStream(ChannelHandle self);

/* Clears the data in the channel */
void Channel_clear(ChannelHandle self);

/**
 *
 * @param data Array to receive the data
 * @param size Max size of array
 * @return -1 if the requested amount of data is not big enough. Else the amount of actaully written data points */
int Channel_read(ChannelHandle self, float data[], size_t size);

/* Returns the handle of the buffer */
FloatRingBufferHandle Channel_getBuffer(ChannelHandle self);

/* Returns the capacity of the Channel */
size_t Channel_getAmountOfUsedData(ChannelHandle self);

#endif
