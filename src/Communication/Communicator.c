/*!****************************************************************************************************************************************
 * @file         Communicator.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Communication/Communicator.h>
#include <stdlib.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommunicatorPrivateData{
    ICommunicatorHandle communicator;

    IRunnable rxRunnable;
    IRunnable txRunnable;

} CommunicatorPrivateData;

/******************************************************************************
 Private functions
******************************************************************************/
static void runRx(IRunnableHandle runnable){
    CommunicatorHandle self = (CommunicatorHandle) runnable->handle;

    self->communicator->runRx(self->communicator);
}

static void runTx(IRunnableHandle runnable){
    CommunicatorHandle self = (CommunicatorHandle) runnable->handle;

    self->communicator->runTx(self->communicator);
}

/******************************************************************************
 Public functions
******************************************************************************/
CommunicatorHandle Communicator_create(ICommunicatorHandle communicator){

    CommunicatorHandle self = malloc(sizeof(CommunicatorPrivateData));

    self->communicator = communicator;

    self->rxRunnable.handle = self;
    self->txRunnable.handle = self;
    self->rxRunnable.run = &runRx;
    self->txRunnable.run = &runTx;

    return self;
}

IRunnableHandle Communicator_getRxRunnable(CommunicatorHandle self){
    return &self->rxRunnable;
}

IRunnableHandle Communicator_getTxRunnable(CommunicatorHandle self){
    return &self->txRunnable;
}

void Communicator_destroy(CommunicatorHandle self){

    free(self);
    self = NULL;
}
