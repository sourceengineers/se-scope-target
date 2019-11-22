/*!****************************************************************************************************************************************
 * @file         EthernetJson.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include "Scope/Communication/Interfaces/EthernetJson.h"
#include "Scope/GeneralPurpose/IByteStream.h"
#include "Scope/GeneralPurpose/IRunnable.h"

#include "Scope/Communication/ICommunicator.h"
#include "Scope/GeneralPurpose/IObserver.h"

#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __EthernetJsonPrivateData{
    ICommunicator communicator;
    ITransceiver transceiver;

    IByteStreamHandle input;
    IByteStreamHandle output;

    bool txPendingToValidateAndTransmit;
    bool rxDataReady;
    TransmitCallback callback;

    IObserverHandle rxObserver;
    IObserver txObserver;

    IRunnable rxRunnable;
    IRunnable txRunnable;
} EthernetJsonPrivateData;

static bool runRx(IRunnableHandle runnable);

static bool runTx(IRunnableHandle runnable);

static void update(IObserverHandle observer, void *state);

static void attachObserver(ICommunicatorHandle communicator, IObserverHandle observer);

static IObserverHandle getObserver(ICommunicatorHandle communicator);

static IRunnableHandle getRxRunnable(ICommunicatorHandle communicator);

static IRunnableHandle getTxRunnable(ICommunicatorHandle communicator);

/******************************************************************************
 Private functions
******************************************************************************/
static bool runRx(IRunnableHandle runnable) {
    EthernetJsonHandle self = (EthernetJsonHandle) runnable->handle;

    if (self->rxDataReady == false) {
        return false;
    }

    self->rxDataReady = false;
    self->rxObserver->update(self->rxObserver, NULL);

    return true;
}

static bool runTx(IRunnableHandle runnable) {
    EthernetJsonHandle self = (EthernetJsonHandle) runnable->handle;

    if (self->txPendingToValidateAndTransmit == false) {
        return false;
    }

    if (self->output->length(self->output) <= 0) {
        return false;
    }

    self->output->writeByte(self->output, '\0');

    self->callback(&self->transceiver);
    self->txPendingToValidateAndTransmit = false;

    return true;
}

static void update(IObserverHandle observer, void *state) {
    EthernetJsonHandle self = (EthernetJsonHandle) observer->handle;
    self->txPendingToValidateAndTransmit = true;
}

static void attachObserver(ICommunicatorHandle communicator, IObserverHandle observer) {
    EthernetJsonHandle self = (EthernetJsonHandle) communicator->handle;
    self->rxObserver = observer;
}

static IObserverHandle getObserver(ICommunicatorHandle communicator) {
    EthernetJsonHandle self = (EthernetJsonHandle) communicator->handle;
    return &self->txObserver;
}

static IRunnableHandle getRxRunnable(ICommunicatorHandle communicator) {
    EthernetJsonHandle self = (EthernetJsonHandle) communicator->handle;
    return &self->rxRunnable;
}

static IRunnableHandle getTxRunnable(ICommunicatorHandle communicator) {
    EthernetJsonHandle self = (EthernetJsonHandle) communicator->handle;
    return &self->txRunnable;
}

void get(ITransceiverHandle transceiver, uint8_t *data, size_t length) {
    EthernetJsonHandle self = (EthernetJsonHandle) transceiver->handle;
    self->output->read(self->output, data, length);
}

size_t outputSize(ITransceiverHandle transceiver) {
    EthernetJsonHandle self = (EthernetJsonHandle) transceiver->handle;
    return self->output->length(self->output);
}

void put(ITransceiverHandle transceiver, uint8_t *data, size_t length) {
    EthernetJsonHandle self = (EthernetJsonHandle) transceiver->handle;
    self->input->write(self->input, data, length);
    self->input->writeByte(self->input, '\0');
    self->rxDataReady = true;
}

void resetInput(ITransceiverHandle transceiver) {
    EthernetJsonHandle self = (EthernetJsonHandle) transceiver->handle;
    self->input->flush(self->input);
}

void resetOutput(ITransceiverHandle transceiver) {
    EthernetJsonHandle self = (EthernetJsonHandle) transceiver->handle;
    self->output->flush(self->output);
}


/******************************************************************************
 Public functions
******************************************************************************/
EthernetJsonHandle
EthernetJson_create(TransmitCallback callback, IByteStreamHandle input, IByteStreamHandle output) {

    EthernetJsonHandle self = malloc(sizeof(EthernetJsonPrivateData));
    assert(self);

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

    self->transceiver.handle = self;
    self->transceiver.get = &get;
    self->transceiver.outputSize = &outputSize;
    self->transceiver.put = &put;
    self->transceiver.resetInput = &resetInput;
    self->transceiver.resetOutput = &resetOutput;

    self->txPendingToValidateAndTransmit = false;
    self->rxDataReady = false;
    self->input = input;
    self->output = output;

    return self;
}

ICommunicatorHandle EthernetJson_getCommunicator(EthernetJsonHandle self) {
    return &self->communicator;
}

ITransceiverHandle EthernetJson_getTransceiver(EthernetJsonHandle self) {
    return &self->transceiver;
}

void EthernetJson_getTxData(EthernetJsonHandle self, uint8_t *data, size_t length) {
    self->output->read(self->output, data, length);
}

size_t EthernetJson_amountOfTxDataPending(EthernetJsonHandle self) {
    return self->output->length(self->output);
}

void EthernetJson_putRxData(EthernetJsonHandle self, uint8_t *data, size_t length) {
    self->input->write(self->input, data, length);
    self->input->writeByte(self->input, '\0');
    self->rxDataReady = true;
}

void EthernetJson_destroy(EthernetJsonHandle self) {
    free(self);
    self = NULL;
}
