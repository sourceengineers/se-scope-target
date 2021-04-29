/*!*****************************************************************************
 * @file         Serializer.h
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
 * @brief        The serialization is responsible to pack and unpack the data.
 *               The protocol that will be used, can be specified by implementing
 *               the IPacker and IUnpacker interfaces for the specific protocol.
 *
 ******************************************************************************/

#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <se-lib-c/util/runnable/IRunnable.h>

#include "Scope/Communication/ICommunicator.h"
#include "Scope/Control/IPacker.h"
#include "Scope/Control/IUnpacker.h"
#include <se-lib-c/util/observer/IObserver.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __SerializerPrivateData* SerializerHandle;

/******************************************************************************
 Public functions
******************************************************************************/
/**
 *
 * @param maxChannels
 * @param maxAddresses
 * @param output
 * @param input
 * @return
 */
SerializerHandle Serializer_create(size_t maxChannels, size_t maxAddresses, IByteStreamHandle output,
                                   IByteStreamHandle input);

/**
 * Returns the runnable for the Rx path
 * @param self
 * @return
 */
IRunnableHandle Serializer_getRxRunnable(SerializerHandle self);

/**
 * Returns the runnable for the Tx path
 * @param self
 * @return
 */
IRunnableHandle Serializer_getTxRunnable(SerializerHandle self);

/**
 * Attach the server for the Tx communication path
 * @param self
 * @param observer
 */
void Serializer_attachCommunicationObserver(SerializerHandle self, IObserverHandle observer);

/**
 * Attach the server for the Rx path
 * @param self
 * @param observer
 */
void Serializer_attachControlObserver(SerializerHandle self, IObserverHandle observer);

/**
 * Returns the observer which will call the packer
 * @param self
 * @return
 */
IObserverHandle Serializer_getPackObserver(SerializerHandle self);

/**
 * Return the observer which will call the unpacker
 * @param self
 * @return
 */
IObserverHandle Serializer_getUnpackObserver(SerializerHandle self);

/**
 *
 * @param self
 * @return
 */
IPackerHandle Serializer_getPacker(SerializerHandle self);

/**
 *
 * @param self
 * @return
 */
IUnpackerHandle Serializer_getUnpacker(SerializerHandle self);

/**
 *
 * @param amountOfChannels
 * @param sizeOfChannels
 * @param addressesInAddressAnnouncer
 * @return
 */
size_t Serializer_txCalculateBufferSize(size_t amountOfChannels, size_t sizeOfChannels, size_t addressesInAddressAnnouncer);

/**
 *
 * @return
 */
size_t Serializer_rxCalculateBufferSize(size_t maxChannels);

/**
 * Deconstructor
 * @param self
 */
void Serializer_destroy(SerializerHandle self);

#endif


