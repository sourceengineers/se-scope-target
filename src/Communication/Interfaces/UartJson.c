/*!****************************************************************************************************************************************
 * @file         UartJson.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Communication/Interfaces/UartJson.h>
#include <Scope/Communication/ICommunicator.h>
#include <Scope/GeneralPurpose/ByteRingBuffer.h>
#include <Scope/GeneralPurpose/IByteStream.h>
#include <Scope/GeneralPurpose/IObserver.h>
#include <Scope/GeneralPurpose/IRunnable.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef enum {
    TRANSPORT_NOT_FOUND, CHECKSUM_FAULTY, CHECKSUM_OK
} ChecksumCheck;

/******************************************************************************
 Define private data
******************************************************************************/
static const char *KEYWORD_TRANSPORT = "transport:";
static const size_t KEYWORD_TRANSPORT_LENGTH = 10;
static const size_t CHECKSUM_LENGTH = 2;

/* Class data */
typedef struct __UartJsonPrivateData {
    ICommunicator communicator;

    UartTransmitCallback callback;
    IByteStreamHandle input;
    IByteStreamHandle output;
    ByteRingBufferHandle transportBuffer;

    uint8_t inputChecksum;
    bool rxDataReady;
    bool txPendingToValidateAndTransmit;

    IObserverHandle rxObserver;
    IObserver txObserver;

    IRunnable rxRunnable;
    IRunnable txRunnable;

} UartJsonPrivateData;

static bool runRx(IRunnableHandle runnable);

static bool runTx(IRunnableHandle runnable);

static void createOutputChecksum(UartJsonHandle self, char *data);

static ChecksumCheck validateInput(UartJsonHandle self);

static void update(IObserverHandle observer, void *state);

static void attachObserver(ICommunicatorHandle communicator, IObserverHandle observer);

static IObserverHandle getObserver(ICommunicatorHandle communicator);

static IRunnableHandle getRxRunnable(ICommunicatorHandle communicator);

static IRunnableHandle getTxRunnable(ICommunicatorHandle communicator);

/******************************************************************************
 Private functions
******************************************************************************/
static bool runRx(IRunnableHandle runnable) {
    UartJsonHandle self = (UartJsonHandle) runnable->handle;

    if (self->rxDataReady == false) {
        return false;
    }

    self->rxDataReady = false;
    self->rxObserver->update(self->rxObserver, NULL);

    return true;
}

static bool runTx(IRunnableHandle runnable) {
    UartJsonHandle self = (UartJsonHandle) runnable->handle;

    if (self->txPendingToValidateAndTransmit == false) {
        return false;
    }

    if (self->output->length(self->output) <= 0) {
        return false;
    }

    char *formatedChecksum;
    formatedChecksum = (char *) "00\0";
    //createOutputChecksum(self, formatedChecksum);
    self->output->write(self->output, (const uint8_t *) KEYWORD_TRANSPORT, KEYWORD_TRANSPORT_LENGTH);
    self->output->write(self->output, (const uint8_t *) formatedChecksum, CHECKSUM_LENGTH + 1);
    self->txPendingToValidateAndTransmit = false;

    self->callback(self);

    return true;
}

static void createOutputChecksum(UartJsonHandle self, char *data) {

    size_t length = self->output->length(self->output);
    uint8_t checksum = 0;

    for (int i = 0; i < length; ++i) {

        uint8_t dataChunk = self->output->readByte(self->output);
        checksum += dataChunk;
        self->output->writeByte(self->output, dataChunk);
    }

    sprintf(data, "%02X", checksum);
}

static ChecksumCheck validateInput(UartJsonHandle self) {

    uint8_t checkData[KEYWORD_TRANSPORT_LENGTH + CHECKSUM_LENGTH];

    ByteRingBuffer_readNoPosInc(self->transportBuffer, checkData, KEYWORD_TRANSPORT_LENGTH + CHECKSUM_LENGTH);

    if (strncmp((const char *) checkData, KEYWORD_TRANSPORT, KEYWORD_TRANSPORT_LENGTH) != 0) {
        return TRANSPORT_NOT_FOUND;
    }

    /* One byte in size has to be added, since sprintf wants to add a deelimiter */
    char checksum[CHECKSUM_LENGTH + 1];
    sprintf(checksum, "%02X", self->inputChecksum);

    if (strncmp((const char *) &checkData[KEYWORD_TRANSPORT_LENGTH], checksum, CHECKSUM_LENGTH) != 0) {
        return CHECKSUM_FAULTY;
    }

    return CHECKSUM_OK;
}

static void update(IObserverHandle observer, void *state) {
    UartJsonHandle self = (UartJsonHandle) observer->handle;
    self->txPendingToValidateAndTransmit = true;
}

static void attachObserver(ICommunicatorHandle communicator, IObserverHandle observer) {
    UartJsonHandle self = (UartJsonHandle) communicator->handle;
    self->rxObserver = observer;
}

static IObserverHandle getObserver(ICommunicatorHandle communicator) {
    UartJsonHandle self = (UartJsonHandle) communicator->handle;
    return &self->txObserver;
}

static IRunnableHandle getRxRunnable(ICommunicatorHandle communicator) {
    UartJsonHandle self = (UartJsonHandle) communicator->handle;
    return &self->rxRunnable;
}

static IRunnableHandle getTxRunnable(ICommunicatorHandle communicator) {
    UartJsonHandle self = (UartJsonHandle) communicator->handle;
    return &self->txRunnable;
}

/******************************************************************************
 Public functions
******************************************************************************/
UartJsonHandle UartJson_create(UartTransmitCallback callback, IByteStreamHandle input, IByteStreamHandle output) {

    UartJsonHandle self = malloc(sizeof(UartJsonPrivateData));
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

    self->rxRunnable.run = &runRx;
    self->txRunnable.run = &runTx;

    self->transportBuffer = ByteRingBuffer_create(KEYWORD_TRANSPORT_LENGTH + CHECKSUM_LENGTH);
    self->callback = callback;
    self->input = input;
    self->output = output;

    self->rxDataReady = false;
    UartJson_resetTx(self);
    UartJson_resetRx(self);
    return self;
}

ICommunicatorHandle UartJson_getCommunicator(UartJsonHandle self) {
    return &self->communicator;
}

void UartJson_getTxData(UartJsonHandle self, uint8_t *data, size_t length) {
    self->output->read(self->output, data, length);
}

size_t UartJson_amountOfTxDataPending(UartJsonHandle self) {
    return self->output->length(self->output);
}

void UartJson_resetRx(UartJsonHandle self) {
    ByteRingBuffer_clear(self->transportBuffer);
    self->input->flush(self->input);
    self->inputChecksum = 0;
    self->rxDataReady = false;
}

void UartJson_resetTx(UartJsonHandle self) {
    self->output->flush(self->output);
    self->txPendingToValidateAndTransmit = false;
}

void UartJson_putRxData(UartJsonHandle self, uint8_t *data, size_t length) {

    if (self->rxDataReady == true) {
        return;
    }

    for (int i = 0; i < length; ++i) {

        if (data[i] == '\0') {
            continue;
        }

        if (ByteRingBuffer_write(self->transportBuffer, &data[i], 1) == -1) {
            uint8_t transportByte;
            ByteRingBuffer_read(self->transportBuffer, &transportByte, 1);
            ByteRingBuffer_write(self->transportBuffer, &data[i], 1);

            self->inputChecksum += transportByte;

            self->input->writeByte(self->input, transportByte);

            ChecksumCheck res = validateInput(self);

            if (res == CHECKSUM_OK) {
                self->input->writeByte(self->input, (uint8_t) '\0');
                ByteRingBuffer_clear(self->transportBuffer);
                self->rxDataReady = true;
                self->inputChecksum = 0;
                break;
            } else if (res == CHECKSUM_FAULTY) {
                UartJson_resetRx(self);
                break;
            } else {
                continue;
            }
        }
    }
}

void UartJson_destroy(UartJsonHandle self) {
    ByteRingBuffer_destroy(self->transportBuffer);

    free(self);
    self = NULL;
}
