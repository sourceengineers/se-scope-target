/*!****************************************************************************************************************************************
 * @file         ScopeBuilder.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include "Scope/Builders/ScopeBuilder.h"

#include "Scope/Control/Controller.h"
#include "Scope/Core/Scope.h"
#include "Scope/Serialisation/Serializer.h"

#include <stdlib.h>
#include <assert.h>
#include <Scope/Serialisation/JsonParser/JsonUnpacker.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __ScopeBuilderPrivateData{
    IByteStreamHandle input;
    IByteStreamHandle output;
    ICommunicatorHandle communicator;
    ScopeHandle scope;

    JsonUnpackerHandle unpacker;
    ControllerHandle controller;
    SerializerHandle serializer;
    AnnounceStorageHandle announceStorage;

    IMutexHandle dataMutex;
    IMutexHandle configMutex;
    size_t maxAddresses;
    uint32_t* timestamp;
    size_t amountOfChannels;
    size_t sizeOfChannels;

} ScopeBuilderPrivateData;

/******************************************************************************
 Public functions
******************************************************************************/
ScopeBuilderHandle ScopeBuilder_create(void){

    ScopeBuilderHandle self = malloc(sizeof(ScopeBuilderPrivateData));
    assert(self);

    self->input = NULL;
    self->output = NULL;
    self->communicator = NULL;
    self->scope = NULL;
    self->controller = NULL;
    self->serializer = NULL;
    self->timestamp = NULL;
    self->announceStorage = NULL;

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

    /* Create layers */
    self->scope = Scope_create(self->sizeOfChannels, self->amountOfChannels, self->timestamp);
    self->unpacker = JsonUnpacker_create(self->input);

    self->serializer = Serializer_create(self->amountOfChannels, self->maxAddresses, self->output);
    self->controller = Controller_create(Scope_getIScope(self->scope), Serializer_getPacker(self->serializer), JsonUnpacker_getIUnpacker(self->unpacker), self->announceStorage);

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
