/*!*****************************************************************************
 * @file         Timestamper.h
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
 *               is not valid. In these cases the purchased commercial license is applied.
 *               Please contact us at scope@sourceengineers.com for a commercial license.
 * }
 *
 * @authors      Samuel Schuepbach <samuel.schuepbach@sourceengineers.com>
 *
 * @brief        The timestamper is used to stamp the timestamp stream.
 *               It also handles the time passed since the last stamping and
 *               therefore allows to implement to only run the scope every 10ms
 *               instead of every ms as example.
 * 
 ******************************************************************************/

#ifndef TIMESTAMPER_H_
#define TIMESTAMPER_H_

#include "Scope/GeneralPurpose/DataTypes.h"

#include <se-lib-c/stream/IIntStream.h>

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __TimestamperPrivateData* TimestamperHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/**
 * Constructor
 * @param capacity Capacity of the internal buffer. This should match the size of the channel buffers
 * @param referenceTimestamp Externally controller timestamp. The timestamps will be copied and put into the buffer
 * @return
 */
TimestamperHandle Timestamper_create(size_t capacity, uint32_t* referenceTimestamp);

/**
 * Calculates the elapsed time since the last stamp and the current referenceTimestamp time
 * @param self
 * @return
 */
bool Timestamper_updateElapsedTime(TimestamperHandle self);

/**
 * Copies the current referenceTimestamp and puts it into the buffer
 * @param self
 */
void Timestamper_stamp(TimestamperHandle self);

/**
 * Clears the data in the timestamper
 * @param self
 */
void Timestamper_clear(TimestamperHandle self);

/**
 * Set the state to running
 * @param self
 */
void Timestamper_setStateRunning(TimestamperHandle self);

/**
 * Set the state to stopped
 * @param self
 */
void Timestamper_setStateStopped(TimestamperHandle self);

/**
 * Returns the value of referenceTimestamp
 * @param self
 * @return
 */
uint32_t Timestamper_getCurrentTime(TimestamperHandle self);

/**
 * Returns the value timestamp increment value. This is used to ajust the polling speed. The poll event can
 * be executed only every 10ms instead of every 1ms as example-
 * @param self
 * @return
 */
uint32_t Timestamper_getTimeIncrement(TimestamperHandle self);

/**
 * Configures the time increment. This is used for the cf_t_inc command
 * @param self
 * @param timstampIncrement
 */
void Timestamper_configureTimestampIncrement(TimestamperHandle self, uint32_t timstampIncrement);

/**
 * Checks if a swap event is pending or not.
 * @param self
 * @return
 */
bool Timestamper_swapIsPending(TimestamperHandle self);

/**
 * Swaps the SWAP and the POLL buffers
 * @param self
 */
void Timestamper_swapBuffers(TimestamperHandle self);

/**
 * Returns the handle to the buffer
 * @param self
 * @return
 */
IIntStreamHandle Timestamper_getStream(TimestamperHandle self);

/**
 * Deconstructor
 * @param self
 */
void Timestamper_destroy(TimestamperHandle self);

#endif
