/*!****************************************************************************************************************************************
 * @file         ScopeBuilder.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/ScopeBuilder.h>
#include <Scope/Control/Controller.h>
#include <Scope/Core/Scope.h>
#include <Scope/Serialisation/Serializer.h>

#include <stdlib.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __ScopeBuilderPrivateData{
    IByteStreamHandle input;
    IByteStreamHandle output;
    IPackerHandle packer;
    IUnpackerHandle unpacker;
    ICommunicatorHandle communicator;
    ScopeHandle scope;
    ControllerHandle controller;
    SerializerHandle serializer;
    AddressStorageHandle addressStorage;

    uint32_t* timestamp;
    size_t amountOfChannels;
    size_t sizeOfChannels;

} ScopeBuilderPrivateData;

/******************************************************************************
 Public functions
******************************************************************************/
ScopeBuilderHandle ScopeBuilder_create(void){

    ScopeBuilderHandle self = malloc(sizeof(ScopeBuilderPrivateData));

    self->input = NULL;
    self->output = NULL;
    self->packer = NULL;
    self->unpacker = NULL;
    self->communicator = NULL;
    self->scope = NULL;
    self->controller = NULL;
    self->serializer = NULL;
    self->timestamp = NULL;
    self->addressStorage = NULL;

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

void ScopeBuilder_setParser(ScopeBuilderHandle self, IPackerHandle packer, IUnpackerHandle unpacker){
    self->packer = packer;
    self->unpacker = unpacker;
}

void ScopeBuilder_setCommunication(ScopeBuilderHandle self, ICommunicatorHandle communicator){
    self->communicator = communicator;
}

void ScopeBuilder_setAddressStorage(ScopeBuilderHandle self, AddressStorageHandle addressStorage){
    self->addressStorage = addressStorage;
}

ScopeObject ScopeBuilder_build(ScopeBuilderHandle self){

    ScopeObject obj;

    obj.runScope = NULL;
    obj.runCommandParser = NULL;
    obj.runDataAggregator = NULL;
    obj.runUnpacker = NULL;
    obj.runPacker = NULL;
    obj.runCommunicationRx = NULL;
    obj.runCommunicationTx = NULL;

    if(self->timestamp == NULL){
        return obj;
    }
    if(self->output == NULL){
        return obj;
    }
    if(self->packer == NULL){
        return obj;
    }
    if(self->communicator == NULL){
        return obj;
    }
    if((self->amountOfChannels == 0) || (self->sizeOfChannels == 0)){
        return obj;
    }

    /* Create layers */
    self->scope = Scope_create(self->sizeOfChannels, self->amountOfChannels, self->addressStorage, self->timestamp);
    self->controller = Controller_create(Scope_getIScope(self->scope), self->packer, self->unpacker);
    self->serializer = Serializer_create(self->packer, self->unpacker);

    /* Connect all observers */
    self->communicator->attachObserver(self->communicator, Serializer_getUnpackObserver(self->serializer));
    Serializer_attachControlObserver(self->serializer, Controller_getCommandObserver(self->controller));
    Scope_attachPackObserver(self->scope, Controller_getCommandPackObserver(self->controller));
    Controller_attachPackObserver(self->controller, Serializer_getPackObserver(self->serializer));
    Serializer_attachCommunicationObserver(self->serializer, self->communicator->getObserver(self->communicator));


    obj.scope = self->scope;
    obj.runScope = Scope_getIRunnable(self->scope);
    obj.runCommandParser = Controller_getRxRunnable(self->controller);
    obj.runDataAggregator = Controller_getTxRunnable(self->controller);
    obj.runUnpacker = Serializer_getRxRunnable(self->serializer);
    obj.runPacker = Serializer_getTxRunnable(self->serializer);
    obj.runCommunicationRx = self->communicator->getRxRunnable(self->communicator);
    obj.runCommunicationTx = self->communicator->getTxRunnable(self->communicator);

    return obj;
}

void ScopeBuilder_destroy(ScopeBuilderHandle self){

    Scope_destroy(self->scope);
    Controller_destroy(self->controller);
    Serializer_destroy(self->serializer);
}
