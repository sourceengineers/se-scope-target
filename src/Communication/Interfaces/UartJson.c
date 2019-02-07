/*!****************************************************************************************************************************************
 * @file         UartJson.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Communication/Interfaces/UartJson.h>
#include <Scope/GeneralPurpose/BufferedByteStream.h>
#include <Scope/GeneralPurpose/ByteRingBuffer.h>
#include <memory.h>
#include <stdio.h>

typedef enum {TRANSPORT_NOT_FOUND, CHECKSUM_FAULTY, CHECKSUM_OK} ChecksumCheck;

/******************************************************************************
 Define private data
******************************************************************************/
static const char* KEYWORD_TRANSPORT = "transport:";
static const size_t KEYWORD_TRANSPORT_LENGTH = 10;
static const size_t CHECKSUM_LENGTH = 2;

/* Class data */
typedef struct __UartJsonPrivateData{

    ICommunicator communicator;

    UartTransmitCallback callback;
    IByteStreamHandle input;
    IByteStreamHandle output;

    ByteRingBufferHandle transportBuffer;

    uint8_t inputChecksum;

    bool rxIsValid;
    bool txPendingToValidate;
    bool txPendingToTransmit;
} UartJsonPrivateData;

ChecksumCheck validateInput(UartJsonHandle self);

void createOutputChecksum(UartJsonHandle self, char* data);
/******************************************************************************
 Private functions
******************************************************************************/
bool rxDataReady(ICommunicatorHandle communicator){
    UartJsonHandle self = (UartJsonHandle) communicator->handle;

    return self->rxIsValid;
}

void txReadyToValidate(ICommunicatorHandle communicator){
    UartJsonHandle self = (UartJsonHandle) communicator->handle;
    self->txPendingToValidate = true;
}

void rxDataHasBeenFetched(ICommunicatorHandle communicator){
    UartJsonHandle self = (UartJsonHandle) communicator->handle;
    self->rxIsValid = false;
    self->input->flush(self->input);
}

void runRx(ICommunicatorHandle communicator){
    UartJsonHandle self = (UartJsonHandle) communicator->handle;
}

bool txSendingPending(ICommunicatorHandle communicator){
    UartJsonHandle self = (UartJsonHandle) communicator->handle;
    return self->txPendingToTransmit;
}

void runTx(ICommunicatorHandle communicator){
    UartJsonHandle self = (UartJsonHandle) communicator->handle;

    if(self->txPendingToValidate == false){
        return;
    }

    if(self->txPendingToTransmit == true){
        return;
    }

    if(self->output->length(self->output) <= 0){
        return;
    }

    char formatedChecksum[3];
    createOutputChecksum(self, formatedChecksum);
    self->output->write(self->output, (const uint8_t*) KEYWORD_TRANSPORT, KEYWORD_TRANSPORT_LENGTH);
    self->output->write(self->output, (const uint8_t*)  formatedChecksum, CHECKSUM_LENGTH + 1);
    self->txPendingToValidate = false;
    self->txPendingToTransmit = true;

    self->callback(self);
}

void createOutputChecksum(UartJsonHandle self, char* data){

    size_t length = self->output->length(self->output);
    uint8_t checksum = 0;

    for(int i = 0; i < length; ++i){

        uint8_t dataChunk = self->output->readByte(self->output);
        checksum += dataChunk;
        self->output->writeByte(self->output, dataChunk);
    }

    sprintf(data, "%02X", checksum);
}

ChecksumCheck validateInput(UartJsonHandle self){

    uint8_t checkData[KEYWORD_TRANSPORT_LENGTH + CHECKSUM_LENGTH];

    ByteRingBuffer_readNoPosInc(self->transportBuffer, checkData, KEYWORD_TRANSPORT_LENGTH + CHECKSUM_LENGTH);

    if(strncmp((const char*) checkData, KEYWORD_TRANSPORT, KEYWORD_TRANSPORT_LENGTH) != 0){
        return TRANSPORT_NOT_FOUND;
    }

    /* One byte in size has to be added, since sprintf wants to add a deelimiter */
    char checksum[CHECKSUM_LENGTH + 1];
    sprintf(checksum, "%02X", self->inputChecksum);

    if(strncmp((const char*) &checkData[KEYWORD_TRANSPORT_LENGTH], checksum, CHECKSUM_LENGTH) != 0){
        return CHECKSUM_FAULTY;
    }

    return CHECKSUM_OK;
}

/******************************************************************************
 Public functions
******************************************************************************/
UartJsonHandle UartJson_create(UartTransmitCallback callback, IByteStreamHandle input, IByteStreamHandle output){

    UartJsonHandle self = malloc(sizeof(UartJsonPrivateData));

    self->communicator.handle = self;
    self->communicator.runRx = &runRx;
    self->communicator.runTx = &runTx;
    self->communicator.rxDataReady = &rxDataReady;
    self->communicator.rxDataHasBeenFetched = &rxDataHasBeenFetched;
    self->communicator.txReadyToValidate = &txReadyToValidate;
    self->communicator.txSendingPending = &txSendingPending;

    self->transportBuffer = ByteRingBuffer_create(KEYWORD_TRANSPORT_LENGTH + CHECKSUM_LENGTH);
    self->callback = callback;
    self->input = input;
    self->output = output;

    self->rxIsValid = false;
    UartJson_resetTx(self);
    UartJson_resetRx(self);
    return self;
}

ICommunicatorHandle UartJson_getCommunicator(UartJsonHandle self){
    return &self->communicator;
}

void UartJson_getTxData(UartJsonHandle self, uint8_t* data, size_t length){
    self->output->read(self->output, data, length);
}

size_t UartJson_amountOfTxDataPending(UartJsonHandle self){
    return self->output->length(self->output);
}

void UartJson_resetRx(UartJsonHandle self){
    ByteRingBuffer_clear(self->transportBuffer);
    self->input->flush(self->input);
    self->inputChecksum = 0;
    self->rxIsValid = false;
}

void UartJson_resetTx(UartJsonHandle self){
    self->output->flush(self->output);
    self->txPendingToValidate = false;
    self->txPendingToTransmit = false;
}

void UartJson_putRxData(UartJsonHandle self, uint8_t* data, size_t length){

    if(self->rxIsValid == true){
        return;
    }

    for(int i = 0; i < length; ++i){

        if(data[i] == '\0'){
            continue;
        }

        if(ByteRingBuffer_write(self->transportBuffer, &data[i], 1) == -1){
            uint8_t transportByte;
            ByteRingBuffer_read(self->transportBuffer, &transportByte, 1);
            ByteRingBuffer_write(self->transportBuffer, &data[i], 1);

            self->inputChecksum += transportByte;

            self->input->writeByte(self->input, transportByte);

            ChecksumCheck res = validateInput(self);

            if(res == CHECKSUM_OK){
                self->input->writeByte(self->input, (uint8_t) '\0');
                ByteRingBuffer_clear(self->transportBuffer);
                self->rxIsValid = true;
                self->inputChecksum = 0;
                break;
            }else if(res == CHECKSUM_FAULTY){
                UartJson_resetRx(self);
                break;
            }else{
                continue;
            }
        }
    }
}

void UartJson_destroy(UartJsonHandle self){
    ByteRingBuffer_destroy(self->transportBuffer);

    free(self);
    self = NULL;
}
