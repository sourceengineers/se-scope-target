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


/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __SerializerPrivateData{
    IRunnable runRx;
    IRunnable runTx;
    IPackerHandle packer;
    IUnpackerHandle unpacker;

    MESSAGE_TYPE unpackingPending;
    MESSAGE_TYPE packingPending;

    IObserver packObserver;
    IObserver unpackObserver;

    IObserverHandle controlObserver;
    IObserverHandle communicationObserver;
} SerializerPrivateData;

static bool runRx(IRunnableHandle runnable);

static bool runTx(IRunnableHandle runnable);

static void updateUnpacker(IObserverHandle observer, void* state);

static void updatePacker(IObserverHandle observer, void* state);

/******************************************************************************
 Private functions
******************************************************************************/
static bool runRx(IRunnableHandle runnable){
    SerializerHandle self = (SerializerHandle) runnable->handle;

    if(self->unpackingPending != SE_NONE){
        return false;
    }

    if(self->unpacker == NULL){
        return false;
    }

    bool parsingIsValid = self->unpacker->unpack(self->unpacker, self->unpackingPending);

    // TODO: Add ACK here!
    self->packObserver.update(&self->packObserver, NULL);

    self->unpackingPending = false;

    self->controlObserver->update(self->controlObserver, NULL);

    return true;
}

static bool runTx(IRunnableHandle runnable){
    SerializerHandle self = (SerializerHandle) runnable->handle;

    if(self->packingPending != SE_NONE){
        return false;
    }

    self->packer->pack(self->packer, self->unpackingPending);
    self->packingPending = false;
    self->communicationObserver->update(self->communicationObserver, NULL);

    return true;
}

static void updateUnpacker(IObserverHandle observer, void* state){
    SerializerHandle self = (SerializerHandle) observer->handle;
    self->unpackingPending = *(MESSAGE_TYPE*) state;
}

static void updatePacker(IObserverHandle observer, void* state){
    SerializerHandle self = (SerializerHandle) observer->handle;
    self->packingPending = *(MESSAGE_TYPE*) state;
}

/******************************************************************************
 Public functions
******************************************************************************/
SerializerHandle Serializer_create(IPackerHandle packer, IUnpackerHandle unpacker){

    SerializerHandle self = malloc(sizeof(SerializerPrivateData));
    assert(self);
    self->packer = packer;
    self->unpacker = unpacker;
    self->packObserver.handle = self;
    self->unpackObserver.handle = self;

    self->packObserver.update = &updatePacker;
    self->unpackObserver.update = &updateUnpacker;

    self->runRx.handle = self;
    self->runTx.handle = self;
    self->runRx.run = &runRx;
    self->runTx.run = &runTx;

    self->unpackingPending = false;
    self->packingPending = false;
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

void Serializer_destroy(SerializerHandle self){
    free(self);
    self = NULL;
}
