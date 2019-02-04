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

    IComValidator validator;

    bool txPending;
} EthernetJsonPrivateData;

/******************************************************************************
 Private functions
******************************************************************************/
bool rxDataReady(IComValidatorHandle validator){
    EthernetJsonHandle self = (EthernetJsonHandle) validator->handle;

    return self->input->length(self->input) > 0;
}

void txReadyToValidate(IComValidatorHandle validator){
    EthernetJsonHandle self = (EthernetJsonHandle) validator->handle;
    self->txPending = true;
}

void runRx(ICommunicatorHandle communicator){
    EthernetJsonHandle self = (EthernetJsonHandle) communicator->handle;
}

void runTx(ICommunicatorHandle communicator){
    EthernetJsonHandle self = (EthernetJsonHandle) communicator->handle;

    if(self->txPending == false){
        return;
    }

    if(self->output->length(self->output) <= 0){
        return;
    }

    if(self->callback == NULL){
        return;
    }

    self->callback(self->output);

    self->txPending = false;
}

/******************************************************************************
 Public functions
******************************************************************************/
EthernetJsonHandle EthernetJson_create(TransmitCallback callback, IByteStreamHandle input, IByteStreamHandle output){

    EthernetJsonHandle self = malloc(sizeof(EthernetJsonPrivateData));

    self->communicator.handle = self;
    self->communicator.runRx = &runRx;
    self->communicator.runTx = &runTx;

    self->validator.handle = self;
    self->validator.rxDataReady = rxDataReady;
    self->validator.txReadyToValidate = txReadyToValidate;

    self->callback = callback;
    self->txPending = false;
    self->input = input;
    self->output = output;

    return self;
}

ICommunicatorHandle EthernetJson_getCommunicator(EthernetJsonHandle self){
    return &self->communicator;
}

IComValidatorHandle EthernetJson_getValidator(EthernetJsonHandle self){
    return &self->validator;
}

void EthernetJson_destroy(EthernetJsonHandle self){

    free(self);
    self = NULL;
}
