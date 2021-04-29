/*!*****************************************************************************
 * @file         FramedIO.h
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
 * @brief        Implementation of a communication interface for frame.
 *
 ******************************************************************************/

#ifndef FRAMEDIO_H
#define FRAMEDIO_H

#include <se-lib-c/stream/IByteStream.h>
#include <se-lib-c/util/runnable/IRunnable.h>

#include "Scope/Communication/ITransceiver.h"
#include "Scope/Communication/ICommunicator.h"
#include <se-lib-c/util/observer/IObserver.h>

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __FramedIOPrivateData* FramedIOHandle;

/******************************************************************************
 Public functions
******************************************************************************/
/**
 * Generates the frame communication interface
 * @param callback Callback to the transmit function defined by the user
 * @param input
 * @param output
 * @return
 */
FramedIOHandle FramedIO_create(TransmitCallback callback, IByteStreamHandle input, IByteStreamHandle output);

/**
 * Returns the communicator interface
 * @param self
 * @return
 */
ICommunicatorHandle FramedIO_getCommunicator(FramedIOHandle self);

/**
 * Returns the transceiver interface
 * @param self
 * @return
 */
ITransceiverHandle FramedIO_getTransceiver(FramedIOHandle self);

/**
 * Writes the data to transmit into data
 * @param self
 * @param data Array to which the data will be written
 * @param length Length of the data array. If this is shorter than EthernetJson_amountOfTxDataPending,
 *               nothing will be written into data
 */
void FramedIO_getTxData(FramedIOHandle self, uint8_t* data, size_t length);

/**
 * Returns the amount of data pending to be sent
 * @param self
 * @return
 */
 size_t FramedIO_amountOfTxDataPending(FramedIOHandle self);

/**
 * Writes data into the object
 * @param self
 * @param data data to be written into the stream
 * @param length amount of data to be written into the stream
 */
bool FramedIO_putRxData(FramedIOHandle self, const uint8_t* data, size_t length);

/**
 * Resets the input stream
 * @param self
 */
void FramedIO_resetRx(FramedIOHandle self);

/**
 * Resets the output stream
 * @param self
 */
void FramedIO_resetTx(FramedIOHandle self);

/**
 * Deconstructor
 * @param self
 */
void FramedIO_destroy(FramedIOHandle self);

#endif


