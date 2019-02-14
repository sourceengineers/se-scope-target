/*!****************************************************************************************************************************************
 * @file         Serializer.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Serialisation/Serializer.h>
#include <stdlib.h>
#include <Scope/Control/ParserDefinitions.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __SerializerPrivateData{
    IRunnable runRx;
    IRunnable runTx;
    IPackerHandle packer;
    IUnpackerHandle unpacker;
    ICommunicatorHandle communicator;

} SerializerPrivateData;

/******************************************************************************
 Private functions
******************************************************************************/
static void runRx(IRunnableHandle runnable){
    SerializerHandle self = (SerializerHandle) runnable->handle;

    if(self->communicator->rxDataReady(self->communicator) == false){
        return;
    }

    if(self->unpacker == NULL){
        return;
    }

    if(self->unpacker->streamIsEmpty(self->unpacker) == true){
        return;
    }

    bool parsingIsValid = self->unpacker->unpack(self->unpacker);

    if(parsingIsValid == false){
        self->packer->prepareFlowControl(self->packer, FLOWCONTROL_NAK);
    }else{
        self->packer->prepareFlowControl(self->packer, FLOWCONTROL_ACK);
    }

    self->communicator->rxDataHasBeenFetched(self->communicator);
}

static void runTx(IRunnableHandle runnable){
    SerializerHandle self = (SerializerHandle) runnable->handle;

    if(self->communicator->txSendingPending(self->communicator) == true){
        return;
    }

    self->packer->pack(self->packer);
    self->communicator->txReadyToValidate(self->communicator);
}

/******************************************************************************
 Public functions
******************************************************************************/
SerializerHandle Serializer_create(IPackerHandle packer, IUnpackerHandle unpacker, \
                                                                        ICommunicatorHandle communicator){

    SerializerHandle self = malloc(sizeof(SerializerPrivateData));

    self->packer = packer;
    self->unpacker = unpacker;
    self->communicator = communicator;

    self->runRx.handle = self;
    self->runTx.handle = self;
    self->runRx.run = &runRx;
    self->runTx.run = &runTx;

    return self;
}

IRunnableHandle Serializer_getTxRunnable(SerializerHandle self){
    return &self->runTx;
}

IRunnableHandle Serializer_getRxRunnable(SerializerHandle self){
    return &self->runRx;
}

void Serializer_destroy(SerializerHandle self){
    free(self);
    self = NULL;
}