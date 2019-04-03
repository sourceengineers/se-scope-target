/*!****************************************************************************************************************************************
 * @file         Serializer.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Control/ParserDefinitions.h>
#include <Scope/Serialisation/Serializer.h>
#include <Scope/Control/IPacker.h>
#include <Scope/Control/IUnpacker.h>
#include <Scope/GeneralPurpose/IObserver.h>
#include <Scope/GeneralPurpose/IRunnable.h>

#include <stdbool.h>
#include <stdlib.h>


/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __SerializerPrivateData{
    IRunnable runRx;
    IRunnable runTx;
    IPackerHandle packer;
    IUnpackerHandle unpacker;

    bool unpackingPending;
    bool packingPending;

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

    if(self->unpackingPending == false){
        return false;
    }

    if(self->unpacker == NULL){
        return false;
    }

    bool parsingIsValid = self->unpacker->unpack(self->unpacker);

    if(parsingIsValid == false){
        self->packer->prepareFlowControl(self->packer, FLOWCONTROL_NAK);
        self->packObserver.update(&self->packObserver, NULL);
    }else{
        self->packer->prepareFlowControl(self->packer, FLOWCONTROL_ACK);
        self->packObserver.update(&self->packObserver, NULL);
    }

    self->unpackingPending = false;

    self->controlObserver->update(self->controlObserver, NULL);

    return true;
}

static bool runTx(IRunnableHandle runnable){
    SerializerHandle self = (SerializerHandle) runnable->handle;

    if(self->packingPending == false){
        return false;
    }

    self->packer->pack(self->packer);
    self->packingPending = false;
    self->communicationObserver->update(self->communicationObserver, NULL);

    return true;
}

static void updateUnpacker(IObserverHandle observer, void* state){
    SerializerHandle self = (SerializerHandle) observer->handle;
    self->unpackingPending = true;
}

static void updatePacker(IObserverHandle observer, void* state){
    SerializerHandle self = (SerializerHandle) observer->handle;
    self->packingPending = true;
}

/******************************************************************************
 Public functions
******************************************************************************/
SerializerHandle Serializer_create(IPackerHandle packer, IUnpackerHandle unpacker){

    SerializerHandle self = malloc(sizeof(SerializerPrivateData));

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
