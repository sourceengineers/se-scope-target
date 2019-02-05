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

    IByteStreamHandle input;
    IByteStreamHandle output;

    bool txPendingToValidate;

    EthernetTransmitCallback callback;
} EthernetJsonPrivateData;

/******************************************************************************
 Private functions
******************************************************************************/
bool rxDataReady(ICommunicatorHandle communicator){
    EthernetJsonHandle self = (EthernetJsonHandle) communicator->handle;

    return self->input->length(self->input) > 0;
}

void txReadyToValidate(ICommunicatorHandle communicator){
    EthernetJsonHandle self = (EthernetJsonHandle) communicator->handle;
    self->txPendingToValidate = true;
}

void rxDataHasBeenFetched(ICommunicatorHandle communicator){
    EthernetJsonHandle self = (EthernetJsonHandle) communicator->handle;
    self->input->flush(self->input);
}

void runRx(ICommunicatorHandle communicator){
    EthernetJsonHandle self = (EthernetJsonHandle) communicator->handle;
}

void runTx(ICommunicatorHandle communicator){
    EthernetJsonHandle self = (EthernetJsonHandle) communicator->handle;

    if(self->txPendingToValidate == false){
        return;
    }

    if(self->output->length(self->output) <= 0){
        return;
    }

    self->callback(self);
    self->txPendingToValidate = false;
}

/******************************************************************************
 Public functions
******************************************************************************/
EthernetJsonHandle EthernetJson_create(EthernetTransmitCallback callback, IByteStreamHandle input, IByteStreamHandle output){

    EthernetJsonHandle self = malloc(sizeof(EthernetJsonPrivateData));

    self->communicator.handle = self;
    self->communicator.runRx = &runRx;
    self->communicator.runTx = &runTx;
    self->communicator.rxDataReady = &rxDataReady;
    self->communicator.rxDataHasBeenFetched = &rxDataHasBeenFetched;
    self->communicator.txReadyToValidate = &txReadyToValidate;

    self->txPendingToValidate = false;
    self->input = input;
    self->output = output;

    return self;
}

ICommunicatorHandle EthernetJson_getCommunicator(EthernetJsonHandle self){
    return &self->communicator;
}

void EthernetJson_getTxData(EthernetJsonHandle self, uint8_t* data, size_t length){
    self->output->read(self->output, data, length);
}

size_t EthernetJson_amountOfTxDataPending(EthernetJsonHandle self){
    return self->output->length(self->output);
}

void EthernetJson_putRxData(EthernetJsonHandle self, uint8_t* data, size_t length){
    self->input->write(self->input, data, length);
}

void EthernetJson_destroy(EthernetJsonHandle self){

    free(self);
    self = NULL;
}
