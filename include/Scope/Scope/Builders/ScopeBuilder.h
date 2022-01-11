/*!*****************************************************************************
 * @file         ScopeBuilder.h
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
 * @brief        Build the Scope
 *
 ******************************************************************************/

#ifndef SCOPEBUILDER_H
#define SCOPEBUILDER_H

#include "Scope/Control/AnnounceStorage.h"
#include <se-lib-c/stream/IByteStream.h>
#include <se-lib-c/util/runnable/IRunnable.h>
#include <se-lib-c/logger/Logger.h>
#include "Scope/GeneralPurpose/IMutex.h"
#include "Scope/Core/IScope.h"
#include "Scope/Control/Controller.h"
#include "Scope/Communication/ICommunicator.h"


#include <stddef.h>
#include <stdint.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __ScopeBuilderPrivateData* ScopeBuilderHandle;

/******************************************************************************
 Define interface
******************************************************************************/
typedef struct ScopeRunnableStruct{

    IScopeHandle scope;
    ControllerHandle controller;

    IRunnableHandle runCommunicationRx;
    IRunnableHandle runCommunicationTx;
    IRunnableHandle runUnpacker;
    IRunnableHandle runPacker;
    IRunnableHandle runCommandParser;
    IRunnableHandle runDataAggregator;
    IRunnableHandle runScope;

    IMutexHandle dataMutex;
    IMutexHandle configMutex;

} ScopeRunnable;

/******************************************************************************
 Public functions
******************************************************************************/
/**
 * Constructor
 * @return ScopeBuilderHandle
 */
ScopeBuilderHandle ScopeBuilder_create(void);

/**
 * Builds the scope and returns an object with runnables
 * @param self
 * @return ScopeRunnable
 */
ScopeRunnable ScopeBuilder_build(ScopeBuilderHandle self);

/**
 * Sets the amount and size of channels that have to be build
 * @param self
 * @param amountOfChannels
 * @param sizeOfChannels
 */
void ScopeBuilder_setChannels(ScopeBuilderHandle self, size_t amountOfChannels, size_t sizeOfChannels);

/**
 * Set the used input and output streams
 * @param self
 * @param input Stream which contains the received data
 * @param output Stream which contains the data to be sent
 */
void ScopeBuilder_setStreams(ScopeBuilderHandle self, IByteStreamHandle input, IByteStreamHandle output);

/**
 * Sets the timestamp reference which will be passed on to the scope
 * @param self
 * @param timestamp
 */
/* */
void ScopeBuilder_setTimestampReference(ScopeBuilderHandle self, uint32_t* timestamp);

/**
 * Set the used communication interface
 * @param self
 * @param interface
 */
void ScopeBuilder_setCommunication(ScopeBuilderHandle self, ICommunicatorHandle interface);

/**
 * Add the AnnounceStorage. With the help of the AnnounceStorage, addresses and their aliases can be sent to the host
 * This can be handy when the addresses aren't known at compile time.
 * This is optional.
 * @param self
 * @param announceStorage
 * @param maxAddresses
 */
void ScopeBuilder_setAnnounceStorage(ScopeBuilderHandle self, AnnounceStorageHandle announceStorage, size_t maxAddresses);

/**
 * Appends the mutex which protects the Tx path of the runner. If no mutex is passed,
 * only the ScopeRunner can be used. Otherwise the ScopeThreadRunner can be used.
 * @param self
 * @param mutex
 */
void ScopeBuilder_setDataMutex(ScopeBuilderHandle self, IMutexHandle mutex);

/**
 * Appends the mutex which protects the Rx path of the runner. If no mutex is passed,
 * only the ScopeRunner can be used. Otherwise the ScopeThreadRunner can be used.
 * @param self
 * @param mutex
 */
void ScopeBuilder_setConfigMutex(ScopeBuilderHandle self, IMutexHandle mutex);

/**
 * Appends the mutex which protects the Rx path of the runner. If no mutex is passed,
 * only the ScopeRunner can be used. Otherwise the ScopeThreadRunner can be used.
 * @param self
 * @param mutex
 */
void ScopeBuilder_setLogBuffer(ScopeBuilderHandle self, IByteStreamHandle logByteStream);

/**
 * Deconstructor
 * @param self
 */
void ScopeBuilder_destroy(ScopeBuilderHandle self);


#endif
