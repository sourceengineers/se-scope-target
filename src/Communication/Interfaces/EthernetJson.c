/*!****************************************************************************************************************************************
 * @file         EthernetJson.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Communication/Interfaces/EthernetJson.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __EthernetJsonPrivateData{

    ICommunicator communicator;

    TransmitCallback callback;
    IByteStreamHandle input;
    IByteStreamHandle output;

} EthernetJsonPrivateData;

/******************************************************************************
 Private functions
******************************************************************************/
void runRx(ICommunicatorHandle communicator){
    EthernetJsonHandle self = (EthernetJsonHandle) communicator->handle;
}

void runTx(ICommunicatorHandle communicator){
    EthernetJsonHandle self = (EthernetJsonHandle) communicator->handle;

    if(self->output->length(self->output) <= 0){
        return;
    }

    if(self->callback == NULL){
        return;
    }

    self->callback(self->output);
}

/******************************************************************************
 Public functions
******************************************************************************/
EthernetJsonHandle EthernetJson_create(TransmitCallback callback, IByteStreamHandle input, IByteStreamHandle output){

    EthernetJsonHandle self = malloc(sizeof(EthernetJsonPrivateData));

    self->communicator.handle = self;
    self->communicator.runRx = &runRx;
    self->communicator.runTx = &runTx;

    self->callback = callback;
    self->input = input;
    self->output = output;

    return self;
}

ICommunicatorHandle EthernetJson_getCommunicator(EthernetJsonHandle self){
    return &self->communicator;
}

void EthernetJson_destroy(EthernetJsonHandle self){

    free(self);
    self = NULL;
}
