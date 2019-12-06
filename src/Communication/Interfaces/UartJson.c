/*!****************************************************************************************************************************************
 * @file         UartJson.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include "Scope/Communication/Interfaces/UartJson.h"
#include "Scope/GeneralPurpose/IRunnable.h"
#include "Scope/GeneralPurpose/IByteStream.h"

#include "Scope/Communication/ICommunicator.h"
#include "Scope/GeneralPurpose/ByteRingBuffer.h"
#include "Scope/GeneralPurpose/IObserver.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <Scope/Core/ScopeTypes.h>

#define LENGTH_OF_LENGTH_FIELD 4
#define LENGTH_OF_CHECKSUM_FIELD 2

typedef enum{

    START_FLAG,
    TYPE,
    LENGTH,
    DATA,
    CHECKSUM,
    END_FLAG

} FrameState;

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __UartJsonPrivateData{
    ICommunicator communicator;
    ITransceiver transceiver;

    TransmitCallback callback;
    IByteStreamHandle input;
    IByteStreamHandle output;

    bool txPendingToValidateAndTransmit;
    MessageType txType;
    ByteRingBufferHandle frameHead;
    ByteRingBufferHandle frameTail;


    IObserverHandle rxObserver;
    IObserver txObserver;
    IRunnable rxRunnable;
    IRunnable txRunnable;

    bool rxDataReady;
    uint32_t rxExpectedLength;
    MessageType rxType;
    size_t rxStateByteCount;
    uint16_t rxChecksum;
    uint16_t rxChecksumCompare;
    FrameState rxFrameState;

} UartJsonPrivateData;

static bool runRx(IRunnableHandle runnable);

static bool runTx(IRunnableHandle runnable);

static void update(IObserverHandle observer, void* state);

static void attachObserver(ICommunicatorHandle communicator, IObserverHandle observer);

static IObserverHandle getObserver(ICommunicatorHandle communicator);

static IRunnableHandle getRxRunnable(ICommunicatorHandle communicator);

static IRunnableHandle getTxRunnable(ICommunicatorHandle communicator);

static bool checkStartFlag(UartJsonHandle self, const uint8_t* data, uint32_t length, uint32_t* dataOffset);

static bool checkType(UartJsonHandle self, const uint8_t* data, uint32_t length, uint32_t* dataOffset);

static bool checkLength(UartJsonHandle self, const uint8_t* data, uint32_t length, uint32_t* dataOffset);

static bool checkData(UartJsonHandle self, const uint8_t* data, uint32_t length, uint32_t* dataOffset);

static bool checkChecksum(UartJsonHandle self, const uint8_t* data, uint32_t length, uint32_t* dataOffset);

static bool checkEndFlag(UartJsonHandle self, const uint8_t* data, uint32_t length, uint32_t* dataOffset);

/******************************************************************************
 Private functions
******************************************************************************/
static bool checkStartFlag(UartJsonHandle self, const uint8_t* data, uint32_t length, uint32_t* dataOffset){
    if(self->rxFrameState == START_FLAG){
        for(; *dataOffset < length; ++*dataOffset){
            if((char) data[*dataOffset] == '['){
                self->rxFrameState = TYPE;
                (*dataOffset)++;
                return true;
            }
        }
    }
    return false;
}

static bool checkType(UartJsonHandle self, const uint8_t* data, uint32_t length, uint32_t* dataOffset){
    if(self->rxFrameState == TYPE){
        if(*dataOffset < length){
            self->rxType = data[*dataOffset];
            (*dataOffset)++;
            self->rxFrameState = LENGTH;
            return true;
        }else{
            // Type wasn't found in frame
            return false;
        }
    }
    return false;
}

static bool checkLength(UartJsonHandle self, const uint8_t* data, uint32_t length, uint32_t* dataOffset){
    // Fetch the length
    if(self->rxFrameState == LENGTH){

        for(; self->rxStateByteCount < LENGTH_OF_LENGTH_FIELD && (*dataOffset) < length; self->rxStateByteCount++){
            uint8_t byte = data[*dataOffset];
            // Shifty shift
            self->rxExpectedLength += byte << (LENGTH_OF_LENGTH_FIELD - self->rxStateByteCount - 1) * 4;
            (*dataOffset)++;
        }

        if(self->rxStateByteCount == LENGTH_OF_LENGTH_FIELD){
            self->rxStateByteCount = 0;
            self->rxFrameState = DATA;
            return true;
        }else{
            // Length field couldn't be found before end of data
            return false;
        }
    }

    return false;
}

static bool checkData(UartJsonHandle self, const uint8_t* data, uint32_t length, uint32_t* dataOffset){
    if(self->rxFrameState == DATA){

        for(; self->input->length(self->input) < self->rxExpectedLength && (*dataOffset) < length; (*dataOffset)++){
            uint8_t byte = data[(*dataOffset)];
            self->rxChecksum += byte;
            self->input->writeByte(self->input, byte);
        }

        size_t writtenData = self->input->length(self->input);


        if(writtenData == self->rxExpectedLength){
            self->rxFrameState = CHECKSUM;
            return true;
        }
        if(length == (*dataOffset)){
            // Reached the end of the array
            return false;
        }
    }
    return false;
}

static bool checkChecksum(UartJsonHandle self, const uint8_t* data, uint32_t length, uint32_t* dataOffset){

    if(self->rxFrameState == CHECKSUM){
        for(; self->rxStateByteCount < LENGTH_OF_CHECKSUM_FIELD && (*dataOffset) < length; (*dataOffset)++){
            uint8_t byte = data[(*dataOffset)];
            self->rxChecksumCompare += (byte << (LENGTH_OF_CHECKSUM_FIELD - self->rxStateByteCount - 1) * 4);
            self->rxStateByteCount++;
        }

        if(self->rxStateByteCount == LENGTH_OF_CHECKSUM_FIELD){
            if(self->rxChecksumCompare == self->rxChecksum){
                self->rxStateByteCount = 0;
                self->rxFrameState = END_FLAG;
                self->rxChecksumCompare = 0;
                self->rxChecksum = 0;
                return true;
            }else{
                UartJson_resetRx(self);
                // Checksum was wrong
                return false;
            }
        }
    }
    return false;
}

static bool checkEndFlag(UartJsonHandle self, const uint8_t* data, uint32_t length, uint32_t* dataOffset){
    if(self->rxFrameState == END_FLAG){
        uint8_t byte = data[(*dataOffset)];
        (*dataOffset)++;
        if(byte == ']'){
            self->rxFrameState = START_FLAG;
            self->rxDataReady = true;
            self->rxExpectedLength = 0;
            return true;
        }else{
            UartJson_resetRx(self);
            // Endflag wasn't found. Which means that something probably went wrong...
            return false;
        }
    }
    return false;
}

static bool runRx(IRunnableHandle runnable){
    UartJsonHandle self = (UartJsonHandle) runnable->handle;

    if(self->rxDataReady == false){
        return false;
    }

    self->rxObserver->update(self->rxObserver, &self->rxType);
    self->rxDataReady = false;
    self->rxType = SE_NONE;

    return true;
}

static bool runTx(IRunnableHandle runnable){
    UartJsonHandle self = (UartJsonHandle) runnable->handle;

    if(self->txPendingToValidateAndTransmit == false){
        return false;
    }

    uint32_t outputBufferLength = self->output->length(self->output);
    if(outputBufferLength == 0){
        return false;
    }

    ByteRingBuffer_write(self->frameHead, (uint8_t*) '[', 1);
    ByteRingBuffer_write(self->frameHead, (uint8_t*) self->txType, 1);

    for(size_t i = 0; i < LENGTH_OF_LENGTH_FIELD; ++i){
        uint8_t byte = (outputBufferLength >> (LENGTH_OF_LENGTH_FIELD - i - 1) * 4) & 0xF;
        ByteRingBuffer_write(self->frameHead, &byte, 1);
    }






    self->txPendingToValidateAndTransmit = false;

    if(self->callback != NULL){
        self->callback(&self->transceiver);
    }

    return true;
}

static void update(IObserverHandle observer, void* state){
    UartJsonHandle self = (UartJsonHandle) observer->handle;
    self->txPendingToValidateAndTransmit = true;
    self->txType = *(MessageType*) state;
}

static void attachObserver(ICommunicatorHandle communicator, IObserverHandle observer){
    UartJsonHandle self = (UartJsonHandle) communicator->handle;
    self->rxObserver = observer;
}

static IObserverHandle getObserver(ICommunicatorHandle communicator){
    UartJsonHandle self = (UartJsonHandle) communicator->handle;
    return &self->txObserver;
}

static IRunnableHandle getRxRunnable(ICommunicatorHandle communicator){
    UartJsonHandle self = (UartJsonHandle) communicator->handle;
    return &self->rxRunnable;
}

static IRunnableHandle getTxRunnable(ICommunicatorHandle communicator){
    UartJsonHandle self = (UartJsonHandle) communicator->handle;
    return &self->txRunnable;
}

void get(ITransceiverHandle transceiver, uint8_t* data, size_t length){
    UartJsonHandle self = (UartJsonHandle) transceiver->handle;
    UartJson_getTxData(self, data, length);
}

size_t outputSize(ITransceiverHandle transceiver){
    UartJsonHandle self = (UartJsonHandle) transceiver->handle;
    return UartJson_amountOfTxDataPending(self);
}

void put(ITransceiverHandle transceiver, uint8_t* data, size_t length){
    UartJsonHandle self = (UartJsonHandle) transceiver->handle;
    UartJson_putRxData(self, data, length);
}

void resetInput(ITransceiverHandle transceiver){
    UartJsonHandle self = (UartJsonHandle) transceiver->handle;
    UartJson_resetRx(self);
}

void resetOutput(ITransceiverHandle transceiver){
    UartJsonHandle self = (UartJsonHandle) transceiver->handle;
    UartJson_resetTx(self);
}

/******************************************************************************
 Public functions
******************************************************************************/
UartJsonHandle UartJson_create(TransmitCallback callback, IByteStreamHandle input, IByteStreamHandle output){

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

    self->callback = callback;
    self->input = input;
    self->output = output;

    self->transceiver.handle = self;
    self->transceiver.get = &get;
    self->transceiver.outputSize = &outputSize;
    self->transceiver.put = &put;
    self->transceiver.resetInput = &resetInput;
    self->transceiver.resetOutput = &resetOutput;

    self->frameHead = ByteRingBuffer_create(6);
    self->frameTail = ByteRingBuffer_create(3);

    self->rxDataReady = false;
    UartJson_resetTx(self);
    UartJson_resetRx(self);
    return self;
}

ICommunicatorHandle UartJson_getCommunicator(UartJsonHandle self){
    return &self->communicator;
}

ITransceiverHandle UartJson_getTransceiver(UartJsonHandle self){
    return &self->transceiver;
}

void UartJson_getTxData(UartJsonHandle self, uint8_t* data, size_t length){
    self->output->read(self->output, data, length);
}

size_t UartJson_amountOfTxDataPending(UartJsonHandle self){
    return self->output->length(self->output);
}

void UartJson_resetRx(UartJsonHandle self){
    self->input->flush(self->input);
    self->rxChecksum = 0;
    self->rxDataReady = false;
    self->rxExpectedLength = 0;
    self->rxType = SE_NONE;
    self->rxStateByteCount = 0;
    self->rxChecksumCompare = 0;
    self->rxFrameState = START_FLAG;
}

void UartJson_resetTx(UartJsonHandle self){
    self->output->flush(self->output);
    self->txPendingToValidateAndTransmit = false;
    self->txType = SE_NONE;
}

bool UartJson_putRxData(UartJsonHandle self, const uint8_t* data, size_t length){

    if(self->rxDataReady == true){
        return false;
    }

    bool frameValid = false;
    uint32_t dataOffSet = 0;

    // Loop over the data until every byte was checked. This is done so if data partial data is appended the framing can
    // handle it. Ex. [protocol][pro. The driver shouldn't discard this.
    do{

        checkStartFlag(self, data, length, &dataOffSet);
        checkType(self, data, length, &dataOffSet);
        checkLength(self, data, length, &dataOffSet);
        checkData(self, data, length, &dataOffSet);
        checkChecksum(self, data, length, &dataOffSet);
        if(checkEndFlag(self, data, length, &dataOffSet)){
            frameValid = true;
        }
    }while(dataOffSet < length);

    return frameValid;
}

void UartJson_destroy(UartJsonHandle self){
    free(self);
    self = NULL;
}
