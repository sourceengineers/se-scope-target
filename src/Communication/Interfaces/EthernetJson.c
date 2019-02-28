/*!****************************************************************************************************************************************
 * @file         EthernetJson.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Communication/Interfaces/EthernetJson.h>
#include <stdlib.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __EthernetJsonPrivateData{
    ICommunicator communicator;

    IByteStreamHandle input;
    IByteStreamHandle output;

    bool txPendingToValidateAndTransmit;
    bool rxDataReady;
    EthernetTransmitCallback callback;

    IObserverHandle rxObserver;
    IObserver txObserver;

    IRunnable rxRunnable;
    IRunnable txRunnable;
} EthernetJsonPrivateData;

/******************************************************************************
 Private functions
******************************************************************************/
void runRx(IRunnableHandle runnable){
    EthernetJsonHandle self = (EthernetJsonHandle) runnable->handle;

    if(self->rxDataReady == false){
        return;
    }

    self->rxDataReady = false;
    self->rxObserver->update(self->rxObserver, NULL);
}

void runTx(IRunnableHandle runnable){
    EthernetJsonHandle self = (EthernetJsonHandle) runnable->handle;

    if(self->txPendingToValidateAndTransmit == false){
        return;
    }

    if(self->output->length(self->output) <= 0){
        return;
    }

    self->output->writeByte(self->output, '\0');

    self->callback(self);
    self->txPendingToValidateAndTransmit = false;
}

void update(IObserverHandle observer, void* state){
    EthernetJsonHandle self = (EthernetJsonHandle) observer->handle;
    self->txPendingToValidateAndTransmit = true;
}

void attachObserver(ICommunicatorHandle communicator, IObserverHandle observer){
    EthernetJsonHandle self = (EthernetJsonHandle) communicator->handle;
    self->rxObserver = observer;
}

IObserverHandle getObserver(ICommunicatorHandle communicator){
    EthernetJsonHandle self = (EthernetJsonHandle) communicator->handle;
    return &self->txObserver;
}

IRunnableHandle getRxRunnable(ICommunicatorHandle communicator){
    EthernetJsonHandle self = (EthernetJsonHandle) communicator->handle;
    return &self->rxRunnable;
}

IRunnableHandle getTxRunnable(ICommunicatorHandle communicator){
    EthernetJsonHandle self = (EthernetJsonHandle) communicator->handle;
    return &self->txRunnable;
}

/******************************************************************************
 Public functions
******************************************************************************/
EthernetJsonHandle
EthernetJson_create(EthernetTransmitCallback callback, IByteStreamHandle input, IByteStreamHandle output){

    EthernetJsonHandle self = malloc(sizeof(EthernetJsonPrivateData));

    self->communicator.handle = self;
    self->rxRunnable.handle = self;
    self->txRunnable.handle = self;
    self->txObserver.handle = self;

    self->communicator.attachObserver = &attachObserver;
    self->communicator.getObserver = &getObserver;
    self->communicator.getRxRunnable = &getRxRunnable;
    self->communicator.getTxRunnable = &getTxRunnable;

    self->txObserver.update = &update;

    self->callback = callback;
    self->rxRunnable.run = &runRx;
    self->txRunnable.run = &runTx;

    self->txPendingToValidateAndTransmit = false;
    self->rxDataReady = false;
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
    self->input->writeByte(self->input, '\0');
    self->rxDataReady = true;
}

void EthernetJson_destroy(EthernetJsonHandle self){
    free(self);
    self = NULL;
}
