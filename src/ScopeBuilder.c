/*!****************************************************************************************************************************************
 * @file         ScopeBuilder.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/ScopeBuilder.h>
#include <Scope/Serialisation/JsonParser/JsonPacker.h>
#include <Scope/Serialisation/JsonParser/JsonUnpacker.h>
#include <Scope/Communication/Interfaces/EthernetJson.h>
/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __ScopeBuilderPrivateData{


    IByteStreamHandle input;
    IByteStreamHandle output;

    IPackerHandle packer;
    IUnpackerHandle unpacker;

    ICommunicatorHandle interface;

    ScopeHandle scope;
    ControllerHandle controller;
    SerializerHandle serializer;
    CommunicatorHandle communicator;

    uint32_t* timestamp;
    AddressStorageHandle addressStorage;

    size_t amountOfChannels;
    size_t sizeOfChannels;

} ScopeBuilderPrivateData;

/******************************************************************************
 Private functions
******************************************************************************/

ScopeBuilderHandle ScopeBuilder_create(){

    ScopeBuilderHandle self = malloc(sizeof(ScopeBuilderPrivateData));

    self->input = NULL;
    self->output = NULL;
    self->packer = NULL;
    self->unpacker = NULL;
    self->interface = NULL;
    self->scope = NULL;
    self->controller = NULL;
    self->serializer = NULL;
    self->communicator = NULL;
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

void ScopeBuilder_setCommunication(ScopeBuilderHandle self, ICommunicatorHandle interface){
    self->interface = interface;
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
    if(self->interface == NULL){
        return obj;
    }
    if((self->amountOfChannels == 0) || (self->sizeOfChannels == 0)){
        return obj;
    }

    self->scope = Scope_create(self->sizeOfChannels, self->amountOfChannels, self->addressStorage, self->timestamp);
    self->controller = Controller_create(Scope_getIScope(self->scope), self->packer, self->unpacker);
    self->serializer = Serializer_create(self->packer, self->unpacker);
    self->communicator = Communicator_create(self->interface);

    obj.scope = self->scope;
    obj.runScope = Scope_getIRunnable(self->scope);
    obj.runCommandParser = Controller_getRxRunnable(self->controller);
    obj.runDataAggregator = Controller_getTxRunnable(self->controller);
    obj.runUnpacker = Serializer_getRxRunnable(self->serializer);
    obj.runPacker = Serializer_getTxRunnable(self->serializer);
    obj.runCommunicationRx = Communicator_getRxRunnable(self->communicator);
    obj.runCommunicationTx = Communicator_getTxRunnable(self->communicator);

    return obj;
}

void ScopeBuilder_destroy(ScopeBuilderHandle self){

    Scope_destroy(self->scope);
    Controller_destroy(self->controller);
    Serializer_destroy(self->serializer);
    Communicator_destroy(self->communicator);

}