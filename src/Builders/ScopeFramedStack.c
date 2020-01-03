/*!****************************************************************************************************************************************
 * @file         ScopeBuilder.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include "Scope/Builders/ScopeFramedStack.h"
#include "Scope/Communication/Interfaces/FramedIO.h"
#include "Scope/Builders/ScopeRunner.h"
#include "Scope/Builders/ScopeThreadRunner.h"
#include "Scope/GeneralPurpose/IMutex.h"

#include <assert.h>
#include <stdlib.h>
#include "Scope/Serialisation/Serializer.h"

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __ScopeFramedStackPrivateData{

    AnnounceStorageHandle announceStorage;
    ITransceiverHandle transceiver;
    ScopeBuilderHandle builder;
    BufferedByteStreamHandle input;
    BufferedByteStreamHandle output;
    FramedIOHandle framedIO;
    ScopeRunnable scopeRunnable;
    size_t outputBufferSize;
    size_t inputBufferSize;

} ScopeFramedStackPrivateData;

/******************************************************************************
 Private functions
******************************************************************************/
ScopeFramedStackHandle ScopeFramedStack_create(ScopeFramedStackConfig config){

    ScopeFramedStackHandle self = malloc(sizeof(ScopeFramedStackPrivateData));
    assert(self);

    /* Let the Packer and Unpacker calculate how much buffer space they are going to use */
    self->outputBufferSize = Serializer_txCalculateBufferSize(config.amountOfChannels, config.sizeOfChannels,
                                                            config.addressesInAddressAnnouncer);
    self->inputBufferSize = Serializer_rxCalculateBufferSize(config.amountOfChannels);

    /* Generate the input and output buffers, based on the previously calculated sizes */
    self->input = BufferedByteStream_create(self->inputBufferSize);
    self->output = BufferedByteStream_create(self->outputBufferSize);

    /* Generate the communication handler */
    self->framedIO = FramedIO_create(config.callback, BufferedByteStream_getIByteStream(self->input),
                                     BufferedByteStream_getIByteStream(self->output));

    self->transceiver = FramedIO_getTransceiver(self->framedIO);

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
    ScopeBuilder_setCommunication(self->builder, FramedIO_getCommunicator(self->framedIO));
    ScopeBuilder_setAnnounceStorage(self->builder, self->announceStorage, config.addressesInAddressAnnouncer);

    /* Build the scope */
    self->scopeRunnable = ScopeBuilder_build(self->builder);

    return self;

}

ScopeFramedStackHandle ScopeFramedStack_createThreadSafe(ScopeFramedStackConfig config, ScopeFramedStackMutex mutexes){

    ScopeFramedStackHandle self = malloc(sizeof(ScopeFramedStackPrivateData));
    assert(self);

    /* Let the Packer and Unpacker calculate how much buffer space they are going to use */
    self->outputBufferSize = Serializer_txCalculateBufferSize(config.amountOfChannels, config.sizeOfChannels,
                                                            config.addressesInAddressAnnouncer);
    self->inputBufferSize = Serializer_rxCalculateBufferSize(config.amountOfChannels);

    /* Generate the input and output buffers, based on the previously calculated sizes */
    self->input = BufferedByteStream_create(self->inputBufferSize);
    self->output = BufferedByteStream_create(self->outputBufferSize);

    /* Generate the communication handler */
    self->framedIO = FramedIO_create(config.callback, BufferedByteStream_getIByteStream(self->input),
                                     BufferedByteStream_getIByteStream(self->output));

    self->transceiver = FramedIO_getTransceiver(self->framedIO);

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
    ScopeBuilder_setCommunication(self->builder, FramedIO_getCommunicator(self->framedIO));
    ScopeBuilder_setAnnounceStorage(self->builder, self->announceStorage, config.addressesInAddressAnnouncer);

    /* Build the scope */
    self->scopeRunnable = ScopeBuilder_build(self->builder);

    return self;
}

void ScopeFramedStack_run(ScopeFramedStackHandle self){
    ScopeRunner_run(self->scopeRunnable);
}

void ScopeFramedStack_runThreadScope(ScopeFramedStackHandle self){
    ScopeThreadRunner_runScope(self->scopeRunnable);
}

void ScopeFramedStack_runThreadStack(ScopeFramedStackHandle self){
    ScopeThreadRunner_runStack(self->scopeRunnable);
}

AnnounceStorageHandle ScopeFramedStack_getAnnounceStorage(ScopeFramedStackHandle self){
    return self->announceStorage;
}

ITransceiverHandle ScopeFramedStack_getTranscevier(ScopeFramedStackHandle self){
    return self->transceiver;
}

void ScopeFramedStack_destroy(ScopeFramedStackHandle self){
    BufferedByteStream_destroy(self->input);
    BufferedByteStream_destroy(self->output);
    FramedIO_destroy(self->framedIO);
    AnnounceStorage_destroy(self->announceStorage);
    ScopeBuilder_destroy(self->builder);
}
