/*!****************************************************************************************************************************************
 * @file         Serializer.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include "Scope/GeneralPurpose/IRunnable.h"

#include "Scope/Control/ParserDefinitions.h"
#include "Scope/Serialisation/Serializer.h"
#include "Scope/Control/IPacker.h"
#include "Scope/Control/IUnpacker.h"
#include "Scope/GeneralPurpose/IObserver.h"

#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <Scope/Serialisation/JsonParser/JsonPacker.h>
#include <Scope/Serialisation/Protobuf/NanopbPacker.h>
#include <Scope/Serialisation/JsonParser/JsonUnpacker.h>


/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __SerializerPrivateData{
    IRunnable runRx;
    IRunnable runTx;
    IPacker packer;
    IUnpackerHandle unpacker;

    MessageType unpackingPending;
    MessageType packingPending;

    IObserver packObserver;
    IObserver unpackObserver;
    JsonPackerHandle jsonPacker;
    NanopbPackerHandle nanopbPacker;
    IByteStreamHandle output;

    IObserverHandle controlObserver;
    IObserverHandle communicationObserver;
} SerializerPrivateData;

static bool runRx(IRunnableHandle runnable);

static bool runTx(IRunnableHandle runnable);

static void updateUnpacker(IObserverHandle observer, void* state);

static void updatePacker(IObserverHandle observer, void* state);

static IPackerHandle getPacker(SerializerHandle self, MessageType type);

/******************************************************************************
 Private functions
******************************************************************************/
static bool runRx(IRunnableHandle runnable){
    SerializerHandle self = (SerializerHandle) runnable->handle;

    if(self->unpackingPending == SE_NONE){
        return false;
    }

    if(self->unpacker == NULL){
        return false;
    }

    bool parsingIsValid = self->unpacker->unpack(self->unpacker, self->unpackingPending);

    if(parsingIsValid){
        self->controlObserver->update(self->controlObserver, &self->unpackingPending);
    }else{
        MessageType type = SE_NAK;
        self->controlObserver->update(self->controlObserver, &type);
    }

    self->unpackingPending = SE_NONE;

    return true;
}

static IPackerHandle getPacker(SerializerHandle self, MessageType type){
    if(type == SC_DATA){
        return NanopbPacker_getIPacker(self->nanopbPacker);
    }else{
        return JsonPacker_getIPacker(self->jsonPacker);
    }
}

static bool runTx(IRunnableHandle runnable){
    SerializerHandle self = (SerializerHandle) runnable->handle;

    if(self->packingPending == SE_NONE){
        return false;
    }

    self->packer.pack(&self->packer, self->packingPending);
    self->communicationObserver->update(self->communicationObserver, &self->packingPending);
    self->packingPending = SE_NONE;

    return true;
}

static void updateUnpacker(IObserverHandle observer, void* state){
    SerializerHandle self = (SerializerHandle) observer->handle;
    self->unpackingPending = *(MessageType*) state;
}

static void updatePacker(IObserverHandle observer, void* state){
    SerializerHandle self = (SerializerHandle) observer->handle;
    self->packingPending = *(MessageType*) state;
}

void pack(IPackerHandle packer, MessageType type){
    SerializerHandle self = (SerializerHandle) packer->handle;
    IPackerHandle currentPacker = getPacker(self, type);
    currentPacker->pack(currentPacker, type);
}

bool isReady(IPackerHandle packer){
    SerializerHandle self = (SerializerHandle) packer->handle;
    return self->packingPending != SE_NONE && self->output->length(self->output) == 0;
}

void addChannel(IPackerHandle packer, FloatRingBufferHandle buffer, const uint32_t channelId){
    SerializerHandle self = (SerializerHandle) packer->handle;
    IPackerHandle currentPacker = NanopbPacker_getIPacker(self->nanopbPacker);
    currentPacker->addChannel(currentPacker, buffer, channelId);
}

void addTimeIncrement(IPackerHandle packer, const uint32_t timeIncrement){
    SerializerHandle self = (SerializerHandle) packer->handle;
    IPackerHandle currentPacker = NanopbPacker_getIPacker(self->nanopbPacker);
    currentPacker->addTimeIncrement(currentPacker, timeIncrement);
}

void addTimestamp(IPackerHandle packer, IIntStreamHandle timestamp){
    SerializerHandle self = (SerializerHandle) packer->handle;
    IPackerHandle currentPacker = NanopbPacker_getIPacker(self->nanopbPacker);
    currentPacker->addTimestamp(currentPacker, timestamp);
}

void addTrigger(IPackerHandle packer, const bool isTriggered, const uint32_t channelId,
                    const uint32_t timestamp, TRIGGER_MODE triggerMode){
    SerializerHandle self = (SerializerHandle) packer->handle;
    IPackerHandle currentPacker = NanopbPacker_getIPacker(self->nanopbPacker);
    currentPacker->addTrigger(currentPacker, isTriggered, channelId, timestamp, triggerMode);
}

void addAddressAnnouncement(IPackerHandle packer, const char* name, const char* type, const ADDRESS_DATA_TYPE address){
    SerializerHandle self = (SerializerHandle) packer->handle;
    IPackerHandle currentPacker = JsonPacker_getIPacker(self->jsonPacker);
    currentPacker->addAddressAnnouncement(currentPacker, name, type, address);
}
void addAnnouncement(IPackerHandle packer, float timeBase, const char* version, size_t maxChannels){
    SerializerHandle self = (SerializerHandle) packer->handle;
    IPackerHandle currentPacker = JsonPacker_getIPacker(self->jsonPacker);
    currentPacker->addAnnouncement(currentPacker, timeBase, version, maxChannels);
}
void reset(IPackerHandle packer){
    return;
};

/******************************************************************************
 Public functions
******************************************************************************/
        SerializerHandle Serializer_create(size_t maxChannels, size_t maxAddresses, IByteStreamHandle output){

    SerializerHandle self = malloc(sizeof(SerializerPrivateData));
    assert(self);

    self->jsonPacker = JsonPacker_create(maxChannels, maxAddresses, output);
    self->nanopbPacker = NanopbPacker_create(maxChannels, maxAddresses, output);

    self->packObserver.handle = self;
    self->unpackObserver.handle = self;
    self->packObserver.update = &updatePacker;
    self->unpackObserver.update = &updateUnpacker;
    self->runRx.handle = self;
    self->runTx.handle = self;
    self->runRx.run = &runRx;
    self->runTx.run = &runTx;
    self->output = output;

    self->unpackingPending = SE_NONE;
    self->packingPending = SE_NONE;
    return self;
}

IRunnableHandle Serializer_getTxRunnable(SerializerHandle self){
    return &self->runTx;
}

IRunnableHandle Serializer_getRxRunnable(SerializerHandle self){
    return &self->runRx;
}

void Serializer_attachCommunicationObserver(SerializerHandle self, IObserverHandle observer){
    self->communicationObserver = observer;
}

void Serializer_attachControlObserver(SerializerHandle self, IObserverHandle observer){
    self->controlObserver = observer;
}

IObserverHandle Serializer_getPackObserver(SerializerHandle self){
    return &self->packObserver;
}

IObserverHandle Serializer_getUnpackObserver(SerializerHandle self){
    return &self->unpackObserver;
}

IPackerHandle Serializer_getPacker(SerializerHandle self){
    return &self->packer;
}


size_t Serializer_txCalculateBufferSize(size_t amountOfChannels, size_t sizeOfChannels, size_t addressesInAddressAnnouncer){
    return NanopbPacker_calculateBufferSize(amountOfChannels, sizeOfChannels, addressesInAddressAnnouncer);
}

size_t Serializer_rxCalculateBufferSize(){
    return JsonUnpacker_calculateBufferSize();
}

void Serializer_destroy(SerializerHandle self){
    free(self);
    self = NULL;
}
