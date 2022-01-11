/*!*****************************************************************************
 * @file         Channel.h
 *
 * @copyright    Copyright (c) 2021 by Source Engineers GmbH. All Rights Reserved.
 *
 * @license {    This file is part of se-scope-target.
 *
 *               se-scope-target is free software; you can redistribute it and/or
 *               modify it under the terms of the GPLv3 General Public License Version 3
 *               as published by the Free Software Foundation.
 *
 *               se-scope-target is distributed in the hope that it will be useful,
 *               but WITHOUT ANY WARRANTY; without even the implied warranty of
 *               MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *               GNU General Public License for more details.
 *
 *               You should have received a copy of the GPLv3 General Public License Version 3
 *               along with se-scope-target.  If not, see <http://www.gnu.org/licenses/>.
 *
 *               In closed source or commercial projects, GPLv3 General Public License Version 3
 *               is not valid. In this case the commercial license received with the purchase
 *               is applied (See SeScopeLicense.pdf).
 *               Please contact us at scope@sourceengineers.com for a commercial license.
 * }
 *
 * @authors      Samuel Schuepbach <samuel.schuepbach@sourceengineers.com>
 *
 * @brief        Impelments the channels, which will be attached to the scope
 * 
 ******************************************************************************/

#ifndef CHANNEL_H_
#define CHANNEL_H_

#include "Scope/GeneralPurpose/DataTypes.h"

#include "Scope/Core/ScopeTypes.h"
#include <se-lib-c/container/FloatRingBuffer.h>
#include <se-lib-c/stream/IFloatStream.h>

#include <stdbool.h>
#include <stddef.h>

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
/**
 * Constructor
 * @param capacity
 * @return
 */
ChannelHandle Channel_create(size_t capacity);

/**
 * Returns the maximum capacity of the channel
 * @param self
 * @return
 */
size_t Channel_getCapacity(ChannelHandle self);

/**
 * Sets the address the channel will poll from
 * @param self
 * @param pollAddress Address the channel will poll from
 * @param pollDataType Type of the data safed in the address
 */
void Channel_setPollAddress(ChannelHandle self, void* pollAddress, DATA_TYPES pollDataType);

/**
 * Returns the address the channel is polling from
 * @param self
 * @return
 */
void* Channel_getPollAddress(ChannelHandle self);

/**
 * Sets the state of the channel to running
 * @param self
 * @return Returns false if it failed
 */
bool Channel_setStateRunning(ChannelHandle self);

/**
 * Sets the state of the channel to stopped
 * @param self
 * @return Returns false if it failed
 */
bool Channel_setStateStopped(ChannelHandle self);

/**
 * Checks if the channel is running
 * @param self
 * @return Returns true if the channel is running. False if not
 */
bool Channel_isRunning(ChannelHandle self);

/**
 * Polls data and writes it into the ringbuffer
 * @param self
 */
void Channel_poll(ChannelHandle self);

/**
 * Returns the stream to the bytes which have to be checked by the trigger.
 * @param self
 * @return
 */
IFloatStreamHandle Channel_getTriggerDataStream(ChannelHandle self);

/**
 * Flushes the channel
 * @param self
 */
void Channel_clear(ChannelHandle self);


/**
 * Checks if the channel is reading to swap
 * @param self
 * @return
 */
bool Channel_swapIsPending(ChannelHandle self);

/**
 * Swaps the internal SWAP and POLL ringbuffers with each other
 * @param self
 */
void Channel_swapBuffers(ChannelHandle self);

/**
 * Reads data from the SWAP ringbuffer
 * @param self
 * @param data Array in to which the data has to be written.
 * @param size Maximum size of data
 * @return The responce of the FloarRingBuffer_read function.
 */
int Channel_read(ChannelHandle self, float data[], size_t size);

/**
 * Returns a reference to the internal SWAP ringbuffer
 * @param self
 * @return
 */
FloatRingBufferHandle Channel_getBuffer(ChannelHandle self);

/**
 * Returns the amount of data in the SWAP buffer
 * @param self
 * @return
 */
size_t Channel_getAmountOfUsedSwapData(ChannelHandle self);

/**
 * Returns the amount of data in the POLL buffer
 * @param self
 * @return
 */
size_t Channel_getAmountOfUsedPollData(ChannelHandle self);

/**
 * Deconstructor
 * @param self
 */
void Channel_destroy(ChannelHandle self);

#endif
