/*!****************************************************************************************************************************************
 * @file         ScopeBuilder.c
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
 *****************************************************************************************************************************************/

#include "Scope/Builders/ScopeBuilder.h"
#include "Scope/Control/Controller.h"
#include "Scope/Core/Scope.h"
#include "Scope/Serialisation/Serializer.h"

#include <stdlib.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __ScopeBuilderPrivateData {
    IByteStreamHandle input;
    IByteStreamHandle output;
    IByteStreamHandle logByteStream;

    ICommunicatorHandle communicator;
    ScopeHandle scope;

    ControllerHandle controller;
    SerializerHandle serializer;
    AnnounceStorageHandle announceStorage;

    IMutexHandle dataMutex;
    IMutexHandle configMutex;
    IMutexHandle logBufferMutex;
    size_t maxAddresses;
    uint32_t* timestamp;
    size_t amountOfChannels;
    size_t sizeOfChannels;

    Message_Priorities priorities;

} ScopeBuilderPrivateData;

/******************************************************************************
 Public functions
******************************************************************************/
ScopeBuilderHandle ScopeBuilder_create(Message_Priorities priorities){

    ScopeBuilderHandle self = malloc(sizeof(ScopeBuilderPrivateData));
    assert(self);

    self->input = NULL;
    self->output = NULL;
    self->communicator = NULL;
    self->scope = NULL;
    self->logByteStream = NULL;
    self->controller = NULL;
    self->serializer = NULL;
    self->timestamp = NULL;
    self->announceStorage = NULL;

    self->priorities = priorities;

    return self;
}

void ScopeBuilder_setChannels(ScopeBuilderHandle self, size_t amountOfChannels, size_t sizeOfChannels){
    self->amountOfChannels = amountOfChannels;
    self->sizeOfChannels = sizeOfChannels;
}

void ScopeBuilder_setStreams(ScopeBuilderHandle self, IByteStreamHandle input, IByteStreamHandle output){
    self->input = input;
    self->output = output;
}

void ScopeBuilder_setTimestampReference(ScopeBuilderHandle self, uint32_t* timestamp){
    self->timestamp = timestamp;
}

void ScopeBuilder_setCommunication(ScopeBuilderHandle self, ICommunicatorHandle communicator){
    self->communicator = communicator;
}

void ScopeBuilder_setAnnounceStorage(ScopeBuilderHandle self, AnnounceStorageHandle announceStorage, size_t maxAddresses){
    self->announceStorage = announceStorage;
    self->maxAddresses = maxAddresses;
}

void ScopeBuilder_setDataMutex(ScopeBuilderHandle self, IMutexHandle mutex){
    self->dataMutex = mutex;
}

void ScopeBuilder_setConfigMutex(ScopeBuilderHandle self, IMutexHandle mutex){
    self->configMutex = mutex;
}

void ScopeBuilder_setLogBuffer(ScopeBuilderHandle self, IByteStreamHandle logByteStream){
	self->logByteStream = logByteStream;
}

ScopeRunnable ScopeBuilder_build(ScopeBuilderHandle self){

    ScopeRunnable runnable;

    runnable.runScope = NULL;
    runnable.runCommandParser = NULL;
    runnable.runDataAggregator = NULL;
    runnable.runUnpacker = NULL;
    runnable.runPacker = NULL;
    runnable.runCommunicationRx = NULL;
    runnable.runCommunicationTx = NULL;


    if(self->timestamp == NULL){
        return runnable;
    }
    if(self->output == NULL){
        return runnable;
    }
    if(self->communicator == NULL){
        return runnable;
    }
    if((self->amountOfChannels == 0) || (self->sizeOfChannels == 0)){
        return runnable;
    }

    if(self->logByteStream == NULL){
    	return runnable;
    }

    /* Create layers */
    self->scope = Scope_create(self->sizeOfChannels, self->amountOfChannels, self->timestamp);

    self->serializer = Serializer_create(self->amountOfChannels, self->maxAddresses, self->output, self->input);
    self->controller = Controller_create(Scope_getIScope(self->scope), Serializer_getPacker(self->serializer),
            Serializer_getUnpacker(self->serializer),
            self->announceStorage,
            self->logByteStream,
            self->priorities,
            self->amountOfChannels);

    /* Connect all observers */
    self->communicator->attachObserver(self->communicator, Serializer_getUnpackObserver(self->serializer));
    Serializer_attachControlObserver(self->serializer, Controller_getCommandObserver(self->controller));
    Scope_attachPackObserver(self->scope, Controller_getCommandPackObserver(self->controller));

    Controller_attachPackObserver(self->controller, Serializer_getPackObserver(self->serializer));
    Serializer_attachCommunicationObserver(self->serializer, self->communicator->getObserver(self->communicator));

    runnable.scope = Scope_getIScope(self->scope);
    runnable.controller = self->controller;
    runnable.dataMutex = self->dataMutex;
    runnable.configMutex = self->configMutex;
    runnable.runScope = Scope_getIRunnable(self->scope);
    runnable.runCommandParser = Controller_getRxRunnable(self->controller);
    runnable.runDataAggregator = Controller_getTxRunnable(self->controller);
    runnable.runUnpacker = Serializer_getRxRunnable(self->serializer);
    runnable.runPacker = Serializer_getTxRunnable(self->serializer);
    runnable.runCommunicationRx = self->communicator->getRxRunnable(self->communicator);
    runnable.runCommunicationTx = self->communicator->getTxRunnable(self->communicator);

    return runnable;
}

void ScopeBuilder_destroy(ScopeBuilderHandle self){
    Scope_destroy(self->scope);
    Controller_destroy(self->controller);
    Serializer_destroy(self->serializer);
}
