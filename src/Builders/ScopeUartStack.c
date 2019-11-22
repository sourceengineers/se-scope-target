/*!****************************************************************************************************************************************
 * @file         ScopeBuilder.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include "Scope/Builders/ScopeUartStack.h"
#include "Scope/Communication/Interfaces/UartJson.h"
#include "Scope/Builders/ScopeRunner.h"
#include "Scope/Builders/ScopeThreadRunner.h"
#include "Scope/GeneralPurpose/IMutex.h"

#include "Scope/Serialisation/JsonParser/JsonPacker.h"
#include "Scope/Serialisation/JsonParser/JsonUnpacker.h"

#include <assert.h>
#include <stdlib.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __ScopeUartStackPrivateData{

    AnnounceStorageHandle announceStorage;
    ITransceiverHandle transceiver;
    ScopeBuilderHandle builder;
    BufferedByteStreamHandle input;
    BufferedByteStreamHandle output;
    JsonUnpackerHandle unpacker;
    JsonPackerHandle packer;
    UartJsonHandle uartJson;
    ScopeRunnable scopeRunnable;
    size_t outputBufferSize;
    size_t inputBufferSize;
    IScopeHandle scope;

} ScopeUartStackPrivateData;

/******************************************************************************
 Private functions
******************************************************************************/
ScopeUartStackHandle ScopeUartStack_create(ScopeUartStackConfig config){

    ScopeUartStackHandle self = malloc(sizeof(ScopeUartStackPrivateData));
    assert(self);

    /* Let the Packer and Unpacker calculate how much buffer space they are going to use */
    self->outputBufferSize = JsonPacker_calculateBufferSize(config.amountOfChannels, config.sizeOfChannels,
                                                            config.addressesInAddressAnnouncer);
    self->inputBufferSize = JsonUnpacker_calculateBufferSize();

    /* Generate the input and output buffers, based on the previously calculated sizes */
    self->input = BufferedByteStream_create(self->inputBufferSize);
    self->output = BufferedByteStream_create(self->outputBufferSize);

    /* Generate the desired packer and unpacker */
    self->packer = JsonPacker_create(config.amountOfChannels, config.addressesInAddressAnnouncer,
                                     BufferedByteStream_getIByteStream(self->output));
    self->unpacker = JsonUnpacker_create(BufferedByteStream_getIByteStream(self->input));

    /* Generate the communication handler */
    self->uartJson = UartJson_create(config.callback, BufferedByteStream_getIByteStream(self->input),
                                     BufferedByteStream_getIByteStream(self->output));

    /* Generate the address storage. The address storage is optional and doesn't have to be used */
    self->announceStorage = AnnounceStorage_create(config.addressesInAddressAnnouncer,
            config.amountOfChannels, config.timebase);

    /* Create the builder itself */
    self->builder = ScopeBuilder_create();

    /* Pass all the wanted elements into the builder */
    ScopeBuilder_setChannels(self->builder, config.amountOfChannels, config.sizeOfChannels);
    ScopeBuilder_setStreams(self->builder, BufferedByteStream_getIByteStream(self->input),
                            BufferedByteStream_getIByteStream(self->output));
    ScopeBuilder_setConfigMutex(self->builder, NULL);
    ScopeBuilder_setDataMutex(self->builder, NULL);
    ScopeBuilder_setTimestampReference(self->builder, config.timestamp);
    ScopeBuilder_setCommunication(self->builder, UartJson_getCommunicator(self->uartJson));
    ScopeBuilder_setParser(self->builder, JsonPacker_getIPacker(self->packer), JsonUnpacker_getIUnpacker(self->unpacker));
    ScopeBuilder_setAnnounceStorage(self->builder, self->announceStorage);

    /* Build the scope */
    self->scopeRunnable = ScopeBuilder_build(self->builder);

    return self;

}

ScopeUartStackHandle ScopeUartStack_createThreadSafe(ScopeUartStackConfig config, ScopeUartStackMutex mutexes){

    ScopeUartStackHandle self = malloc(sizeof(ScopeUartStackPrivateData));
    assert(self);

    /* Let the Packer and Unpacker calculate how much buffer space they are going to use */
    self->outputBufferSize = JsonPacker_calculateBufferSize(config.amountOfChannels, config.sizeOfChannels,
                                                            config.addressesInAddressAnnouncer);
    self->inputBufferSize = JsonUnpacker_calculateBufferSize();

    /* Generate the input and output buffers, based on the previously calculated sizes */
    self->input = BufferedByteStream_create(self->inputBufferSize);
    self->output = BufferedByteStream_create(self->outputBufferSize);

    /* Generate the desired packer and unpacker */
    self->packer = JsonPacker_create(config.amountOfChannels, config.addressesInAddressAnnouncer,
                                     BufferedByteStream_getIByteStream(self->output));
    self->unpacker = JsonUnpacker_create(BufferedByteStream_getIByteStream(self->input));

    /* Generate the communication handler */
    self->uartJson = UartJson_create(config.callback, BufferedByteStream_getIByteStream(self->input),
                                     BufferedByteStream_getIByteStream(self->output));

    self->transceiver = UartJson_getTransceiver(self->uartJson);

    /* Generate the address storage. The address storage is optional and doesn't have to be used */
    self->announceStorage = AnnounceStorage_create(config.addressesInAddressAnnouncer, config.amountOfChannels, config.timebase);

    /* Create the builder itself */
    self->builder = ScopeBuilder_create();

    /* Pass all the wanted elements into the builder */
    ScopeBuilder_setChannels(self->builder, config.amountOfChannels, config.sizeOfChannels);
    ScopeBuilder_setStreams(self->builder, BufferedByteStream_getIByteStream(self->input),
                            BufferedByteStream_getIByteStream(self->output));
    ScopeBuilder_setConfigMutex(self->builder, mutexes.configMutex);
    ScopeBuilder_setDataMutex(self->builder, mutexes.dataMutex);
    ScopeBuilder_setTimestampReference(self->builder, config.timestamp);
    ScopeBuilder_setCommunication(self->builder, UartJson_getCommunicator(self->uartJson));
    ScopeBuilder_setParser(self->builder, JsonPacker_getIPacker(self->packer), JsonUnpacker_getIUnpacker(self->unpacker));
    ScopeBuilder_setAnnounceStorage(self->builder, self->announceStorage);

    /* Build the scope */
    self->scopeRunnable = ScopeBuilder_build(self->builder);

    return self;
}

void ScopeUartStack_run(ScopeUartStackHandle self){
    ScopeRunner_run(self->scopeRunnable);
}

void ScopeUartStack_runThreadScope(ScopeUartStackHandle self){
    ScopeThreadRunner_runScope(self->scopeRunnable);
}

void ScopeUartStack_runThreadStack(ScopeUartStackHandle self){
    ScopeThreadRunner_runStack(self->scopeRunnable);
}

AnnounceStorageHandle ScopeUartStack_getAnnounceStorage(ScopeUartStackHandle self){
    return self->announceStorage;
}

ITransceiverHandle ScopeUartStack_getTranscevier(ScopeUartStackHandle self){
    return self->transceiver;
}

void ScopeUartStack_destroy(ScopeUartStackHandle self){
    BufferedByteStream_destroy(self->input);
    BufferedByteStream_destroy(self->output);
    JsonUnpacker_destroy(self->unpacker);
    JsonPacker_destroy(self->packer);
    UartJson_destroy(self->uartJson);
    AnnounceStorage_destroy(self->announceStorage);
    ScopeBuilder_destroy(self->builder);
}
