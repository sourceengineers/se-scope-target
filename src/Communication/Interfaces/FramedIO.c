/*!****************************************************************************************************************************************
 * @file         FramedIO.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include "Scope/Communication/Interfaces/FramedIO.h"
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
#define START_LENGTH 1
#define END_LENGTH 1
#define TYPE_LENGTH 1

typedef enum{

    START_FLAG,
    TYPE,
    LENGTH,
    DATA,
    CHECKSUM,
    END_FLAG

} RxFramedIOState;

typedef enum{
    HEAD,
    BODY,
    FOOT,
    NONE,
} TxFramedIOState;

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __FramedIOPrivateData{
    ICommunicator communicator;
    ITransceiver transceiver;

    TransmitCallback callback;
    IByteStreamHandle input;
    IByteStreamHandle output;

    ByteRingBufferHandle frameHead;
    ByteRingBufferHandle frameTail;


    IObserverHandle rxObserver;
    IObserver txObserver;
    IRunnable rxRunnable;
    IRunnable txRunnable;

    bool txDataFramingReady;
    MessageType txType;
    uint16_t txChecksum;
    TxFramedIOState txFramedIOState;

    bool rxDataReady;
    uint32_t rxExpectedLength;
    MessageType rxType;
    size_t rxStateByteCount;
    uint16_t rxChecksum;
    uint16_t rxChecksumCompare;
    RxFramedIOState rxFramedIOState;

} FramedIOPrivateData;

static bool runRx(IRunnableHandle runnable);

static bool runTx(IRunnableHandle runnable);

static void update(IObserverHandle observer, void* state);

static void attachObserver(ICommunicatorHandle communicator, IObserverHandle observer);

static IObserverHandle getObserver(ICommunicatorHandle communicator);

static IRunnableHandle getRxRunnable(ICommunicatorHandle communicator);

static IRunnableHandle getTxRunnable(ICommunicatorHandle communicator);

static bool checkStartFlag(FramedIOHandle self, const uint8_t* data, uint32_t length, uint32_t* dataOffset);

static bool checkType(FramedIOHandle self, const uint8_t* data, uint32_t length, uint32_t* dataOffset);

static bool checkLength(FramedIOHandle self, const uint8_t* data, uint32_t length, uint32_t* dataOffset);

static bool checkData(FramedIOHandle self, const uint8_t* data, uint32_t length, uint32_t* dataOffset);

static bool checkChecksum(FramedIOHandle self, const uint8_t* data, uint32_t length, uint32_t* dataOffset);

static bool checkEndFlag(FramedIOHandle self, const uint8_t* data, uint32_t length, uint32_t* dataOffset);

/******************************************************************************
 Private functions
******************************************************************************/
static bool checkStartFlag(FramedIOHandle self, const uint8_t* data, uint32_t length, uint32_t* dataOffset){
    if(self->rxFramedIOState == START_FLAG){
        for(; *dataOffset < length; ++*dataOffset){
            if((char) data[*dataOffset] == '['){
                self->rxFramedIOState = TYPE;
                (*dataOffset)++;
                return true;
            }
        }
    }
    return false;
}

static bool checkType(FramedIOHandle self, const uint8_t* data, uint32_t length, uint32_t* dataOffset){
    if(self->rxFramedIOState == TYPE){
        if(*dataOffset < length){
            self->rxType = data[*dataOffset];
            (*dataOffset)++;
            self->rxFramedIOState = LENGTH;
            return true;
        }else{
            // Type wasn't found in frame
            return false;
        }
    }
    return false;
}

static bool checkLength(FramedIOHandle self, const uint8_t* data, uint32_t length, uint32_t* dataOffset){
    // Fetch the length
    if(self->rxFramedIOState == LENGTH){

        for(; self->rxStateByteCount < LENGTH_OF_LENGTH_FIELD && (*dataOffset) < length; self->rxStateByteCount++){
            uint8_t byte = data[*dataOffset];
            // Shifty shift
            self->rxExpectedLength += byte << (LENGTH_OF_LENGTH_FIELD - self->rxStateByteCount - 1) * 8;
            (*dataOffset)++;
        }

        if(self->rxStateByteCount == LENGTH_OF_LENGTH_FIELD){
            self->rxStateByteCount = 0;
            self->rxFramedIOState = DATA;
            return true;
        }else{
            // Length field couldn't be found before end of data
            return false;
        }
    }

    return false;
}

static bool checkData(FramedIOHandle self, const uint8_t* data, uint32_t length, uint32_t* dataOffset){
    if(self->rxFramedIOState == DATA){

        for(; self->input->length(self->input) < self->rxExpectedLength && (*dataOffset) < length; (*dataOffset)++){
            uint8_t byte = data[(*dataOffset)];
            self->rxChecksum += byte;
            self->input->writeByte(self->input, byte);
        }

        size_t writtenData = self->input->length(self->input);

        if(writtenData == self->rxExpectedLength){
            self->rxFramedIOState = CHECKSUM;
            return true;
        }
        if(length == (*dataOffset)){
            // Reached the end of the array
            return false;
        }
    }
    return false;
}

static bool checkChecksum(FramedIOHandle self, const uint8_t* data, uint32_t length, uint32_t* dataOffset){

    if(self->rxFramedIOState == CHECKSUM){
        for(; self->rxStateByteCount < LENGTH_OF_CHECKSUM_FIELD && (*dataOffset) < length; (*dataOffset)++){
            uint8_t byte = data[(*dataOffset)];
            self->rxChecksumCompare += (byte << (LENGTH_OF_CHECKSUM_FIELD - self->rxStateByteCount - 1) * 8);
            self->rxStateByteCount++;
        }

        if(self->rxStateByteCount == LENGTH_OF_CHECKSUM_FIELD){
            if(self->rxChecksumCompare == self->rxChecksum){
                self->rxStateByteCount = 0;
                self->rxFramedIOState = END_FLAG;
                self->rxChecksumCompare = 0;
                self->rxChecksum = 0;
                return true;
            }else{
                FramedIO_resetRx(self);
                // Checksum was wrong
                return false;
            }
        }
    }
    return false;
}

static bool checkEndFlag(FramedIOHandle self, const uint8_t* data, uint32_t length, uint32_t* dataOffset){
    if(self->rxFramedIOState == END_FLAG){
        uint8_t byte = data[(*dataOffset)];
        (*dataOffset)++;
        if(byte == ']'){
            self->rxFramedIOState = START_FLAG;
            self->rxDataReady = true;
            self->rxExpectedLength = 0;
            return true;
        }else{
            FramedIO_resetRx(self);
            // Endflag wasn't found. Which means that something probably went wrong...
            return false;
        }
    }
    return false;
}

static bool runRx(IRunnableHandle runnable){
    FramedIOHandle self = (FramedIOHandle) runnable->handle;

    if(self->rxDataReady == false){
        return false;
    }

    self->rxObserver->update(self->rxObserver, &self->rxType);
    self->rxDataReady = false;
    self->rxType = SE_NONE;

    return true;
}

static bool runTx(IRunnableHandle runnable){
    FramedIOHandle self = (FramedIOHandle) runnable->handle;

    if(self->txFramedIOState != NONE) {
        return false;
    }

    if(self->txDataFramingReady== false){
        return false;
    }

    uint32_t outputBufferLength = self->output->length(self->output);

    // Write the startbyte in a temporary buffer
    char startByte = '[';
    ByteRingBuffer_write(self->frameHead, (uint8_t*) &startByte, 1);

    // Write the type in a temporary buffer
    ByteRingBuffer_write(self->frameHead, (uint8_t*) &self->txType, 1);

    // Write the length in a temporary buffer
    for(size_t i = 0; i < LENGTH_OF_LENGTH_FIELD; ++i){
        uint8_t byte = (outputBufferLength >> ((LENGTH_OF_LENGTH_FIELD - i - 1) * 8)) & 0xFF;
        ByteRingBuffer_write(self->frameHead, &byte, 1);
    }
    self->txDataFramingReady = false;

    self->txFramedIOState = HEAD;
    if(self->callback != NULL){
        self->callback(&self->transceiver);
    }

    return true;
}

static void update(IObserverHandle observer, void* state){
    FramedIOHandle self = (FramedIOHandle) observer->handle;
    self->txDataFramingReady = true;
    self->txType = *(MessageType*) state;
}

static void attachObserver(ICommunicatorHandle communicator, IObserverHandle observer){
    FramedIOHandle self = (FramedIOHandle) communicator->handle;
    self->rxObserver = observer;
}

static IObserverHandle getObserver(ICommunicatorHandle communicator){
    FramedIOHandle self = (FramedIOHandle) communicator->handle;
    return &self->txObserver;
}

static IRunnableHandle getRxRunnable(ICommunicatorHandle communicator){
    FramedIOHandle self = (FramedIOHandle) communicator->handle;
    return &self->rxRunnable;
}

static IRunnableHandle getTxRunnable(ICommunicatorHandle communicator){
    FramedIOHandle self = (FramedIOHandle) communicator->handle;
    return &self->txRunnable;
}

void get(ITransceiverHandle transceiver, uint8_t* data, size_t length){
    FramedIOHandle self = (FramedIOHandle) transceiver->handle;
    FramedIO_getTxData(self, data, length);
}

size_t outputSize(ITransceiverHandle transceiver){
    FramedIOHandle self = (FramedIOHandle) transceiver->handle;
    return FramedIO_amountOfTxDataPending(self);
}

void put(ITransceiverHandle transceiver, uint8_t* data, size_t length){
    FramedIOHandle self = (FramedIOHandle) transceiver->handle;
    FramedIO_putRxData(self, data, length);
}

void resetInput(ITransceiverHandle transceiver){
    FramedIOHandle self = (FramedIOHandle) transceiver->handle;
    FramedIO_resetRx(self);
}

void resetOutput(ITransceiverHandle transceiver){
    FramedIOHandle self = (FramedIOHandle) transceiver->handle;
    FramedIO_resetTx(self);
}

/******************************************************************************
 Public functions
******************************************************************************/
FramedIOHandle FramedIO_create(TransmitCallback callback, IByteStreamHandle input, IByteStreamHandle output){

    FramedIOHandle self = malloc(sizeof(FramedIOPrivateData));
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

    self->frameHead = ByteRingBuffer_create(START_LENGTH + LENGTH_OF_LENGTH_FIELD + TYPE_LENGTH);
    self->frameTail = ByteRingBuffer_create(LENGTH_OF_CHECKSUM_FIELD + END_LENGTH);

    self->rxDataReady = false;
    FramedIO_resetTx(self);
    FramedIO_resetRx(self);
    return self;
}

ICommunicatorHandle FramedIO_getCommunicator(FramedIOHandle self){
    return &self->communicator;
}

ITransceiverHandle FramedIO_getTransceiver(FramedIOHandle self){
    return &self->transceiver;
}

void FramedIO_getTxData(FramedIOHandle self, uint8_t* data, size_t length){

    if(self->txFramedIOState == NONE){
        return;
    }

    size_t index = 0;

    if(self->txFramedIOState == HEAD){
        for(; index < length; index++){
            uint8_t byte;
            ByteRingBuffer_read(self->frameHead, &byte, 1);
            data[index] = byte;

            if(ByteRingBuffer_getNumberOfUsedData(self->frameHead) == 0){
                self->txFramedIOState = BODY;
                index++;
                break;
            }
        }
    }

    if(self->txFramedIOState == BODY){
        for(; index < length; index++){
            // Looping the bytes in one by one to allow to produce a checksum while doing so.
            size_t bytesLeft = self->output->length(self->output);
            if(bytesLeft > 0){
                uint8_t byte = self->output->readByte(self->output);
                data[index] = byte;
                self->txChecksum += byte;
            } else {
                self->txFramedIOState = FOOT;
                // Currently the checksum is not written in the buffer yet
                uint8_t checksumLeft = (self->txChecksum & 0xFF00) >> 8;
                uint8_t checksumRight = self->txChecksum & 0xFF;
                self->txChecksum = 0;
                ByteRingBuffer_write(self->frameTail, (uint8_t*) &checksumLeft, 1);
                ByteRingBuffer_write(self->frameTail, (uint8_t*) &checksumRight, 1);
                char endByte = ']';
                ByteRingBuffer_write(self->frameTail, (uint8_t*) &endByte, 1);
                break;
            }
        }
    }

    if(self->txFramedIOState == FOOT){
        for(; index < length; index++){

            uint8_t byte;
            ByteRingBuffer_read(self->frameTail, &byte, 1);
            data[index] = byte;
            if(ByteRingBuffer_getNumberOfUsedData(self->frameTail) == 0){
                self->txFramedIOState = NONE;
                index++;
                break;
            }
        }
    }
}

size_t FramedIO_amountOfTxDataPending(FramedIOHandle self){
    if(self->txFramedIOState == HEAD){
        return ByteRingBuffer_getNumberOfUsedData(self->frameHead) + self->output->length(self->output)
               + ByteRingBuffer_getCapacity(self->frameTail);
    }else if(self->txFramedIOState == BODY){
        return self->output->length(self->output) + ByteRingBuffer_getCapacity(self->frameTail);
    }else if(self->txFramedIOState == FOOT){
        return ByteRingBuffer_getNumberOfUsedData(self->frameTail);
    }
    return 0;
}

void FramedIO_resetRx(FramedIOHandle self){
    self->input->flush(self->input);
    self->rxChecksum = 0;
    self->rxDataReady = false;
    self->rxExpectedLength = 0;
    self->rxType = SE_NONE;
    self->rxStateByteCount = 0;
    self->rxChecksumCompare = 0;
    self->rxFramedIOState = START_FLAG;
}

void FramedIO_resetTx(FramedIOHandle self){
    self->output->flush(self->output);
    self->txDataFramingReady = true;
    self->txType = SE_NONE;
    self->txFramedIOState = NONE;
}

bool FramedIO_putRxData(FramedIOHandle self, const uint8_t* data, size_t length){

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

void FramedIO_destroy(FramedIOHandle self){
    ByteRingBuffer_destroy(self->frameHead);
    ByteRingBuffer_destroy(self->frameTail);
    free(self);
    self = NULL;
}