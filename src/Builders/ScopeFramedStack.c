/*!****************************************************************************************************************************************
 * @file         ScopeBuilder.c
 *
 * @copyright    Copyright (c) 2021 by Source Engineers GmbH. All Rights Reserved.
 *
 * @license {    This file is part of se-scope-target.
 *
 *               se-scope-target is free software; you can redistribute it and/or
 *               modify it under the terms of the GPLv3 General Public License Version 3
 *               as published by the Free Software Foundation.
 *
 *               se-scope-target is distributed in the hope that it will be useful,
 *               but WITHOUT ANY WARRANTY; without even the implied warranty of
 *               MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *               GNU General Public License for more details.
 *
 *               You should have received a copy of the GPLv3 General Public License Version 3
 *               along with se-scope-target.  If not, see <http://www.gnu.org/licenses/>.
 *
 *               In closed source or commercial projects, GPLv3 General Public License Version 3
 *               is not valid. In these cases the purchased commercial license is applied.
 *               Please contact us at scope@sourceengineers.com for a commercial license.
 * }
 *
 * @authors      Samuel Schuepbach <samuel.schuepbach@sourceengineers.com>
 *
 *****************************************************************************************************************************************/

#include "Scope/Builders/ScopeFramedStack.h"
#include "Scope/Communication/Interfaces/FramedIO.h"
#include "Scope/Builders/ScopeRunner.h"
#include "Scope/Builders/ScopeThreadRunner.h"
#include <se-lib-c/osal/IMutex.h>

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
ScopeFramedStackHandle ScopeFramedStack_create(ScopeFramedStackConfig scopeConfig,
                                               ScopeFramedStackLogOptions logConfig,
                                               Message_Priorities priorities) {

    ScopeFramedStackHandle self = malloc(sizeof(ScopeFramedStackPrivateData));
    assert(self);

    /* Let the Packer and Unpacker calculate how much buffer space they are going to use */
    self->outputBufferSize = Serializer_txCalculateBufferSize(scopeConfig.amountOfChannels, scopeConfig.sizeOfChannels,
                                                              scopeConfig.addressesInAddressAnnouncer);
    self->inputBufferSize = Serializer_rxCalculateBufferSize(scopeConfig.amountOfChannels);

    /* Generate the input and output buffers, based on the previously calculated sizes */
    self->input = BufferedByteStream_create(self->inputBufferSize);
    self->output = BufferedByteStream_create(self->outputBufferSize);

    /* Generate the communication handler */
    self->framedIO = FramedIO_create(scopeConfig.callback,
                                     BufferedByteStream_getByteRingBufferHandle(self->input),
                                     BufferedByteStream_getByteRingBufferHandle(self->output));

    self->transceiver = FramedIO_getTransceiver(self->framedIO);

    /* Generate the address storage. The address storage is optional and doesn't have to be used */
    self->announceStorage = AnnounceStorage_create(scopeConfig.addressesInAddressAnnouncer,
                                                   scopeConfig.amountOfChannels, scopeConfig.timebase);

    /* Create the builder itself */
    self->builder = ScopeBuilder_create(priorities);

    /* Pass all the wanted elements into the builder */
    ScopeBuilder_setChannels(self->builder, scopeConfig.amountOfChannels, scopeConfig.sizeOfChannels);
    ScopeBuilder_setStreams(self->builder, BufferedByteStream_getIByteStream(self->input),
                            BufferedByteStream_getIByteStream(self->output));
    ScopeBuilder_setConfigMutex(self->builder, NULL);
    ScopeBuilder_setDataMutex(self->builder, NULL);
    ScopeBuilder_setTimestampReference(self->builder, scopeConfig.timestamp);
    ScopeBuilder_setCommunication(self->builder, FramedIO_getCommunicator(self->framedIO));
    ScopeBuilder_setAnnounceStorage(self->builder, self->announceStorage, scopeConfig.addressesInAddressAnnouncer);
    ScopeBuilder_setLogBuffer(self->builder, logConfig.logByteStream);

    /* Build the scope */
    self->scopeRunnable = ScopeBuilder_build(self->builder);

    return self;

}

ScopeFramedStackHandle ScopeFramedStack_createThreadSafe(
		ScopeFramedStackConfig scopeConfig,
		ScopeFramedStackMutex mutexes,
		ScopeFramedStackLogOptions logConfig,
        Message_Priorities priorities){

    ScopeFramedStackHandle self = malloc(sizeof(ScopeFramedStackPrivateData));
    assert(self);

    /* Let the Packer and Unpacker calculate how much buffer space they are going to use */
    self->outputBufferSize = Serializer_txCalculateBufferSize(scopeConfig.amountOfChannels, scopeConfig.sizeOfChannels,
                                                              scopeConfig.addressesInAddressAnnouncer);
    self->inputBufferSize = Serializer_rxCalculateBufferSize(scopeConfig.amountOfChannels);

    /* Generate the input and output buffers, based on the previously calculated sizes */
    self->input = BufferedByteStream_create(self->inputBufferSize);
    self->output = BufferedByteStream_create(self->outputBufferSize);

    /* Generate the communication handler */
    self->framedIO = FramedIO_create(scopeConfig.callback,
                                     BufferedByteStream_getByteRingBufferHandle(self->input),
                                     BufferedByteStream_getByteRingBufferHandle(self->output));

    self->transceiver = FramedIO_getTransceiver(self->framedIO);

    /* Generate the address storage. The address storage is optional and doesn't have to be used */
    self->announceStorage = AnnounceStorage_create(scopeConfig.addressesInAddressAnnouncer, scopeConfig.amountOfChannels, scopeConfig.timebase);

    /* Create the builder itself */
    self->builder = ScopeBuilder_create(priorities);

    /* Pass all the wanted elements into the builder */
    ScopeBuilder_setChannels(self->builder, scopeConfig.amountOfChannels, scopeConfig.sizeOfChannels);
    ScopeBuilder_setStreams(self->builder, BufferedByteStream_getIByteStream(self->input),
                            BufferedByteStream_getIByteStream(self->output));
    ScopeBuilder_setConfigMutex(self->builder, mutexes.configMutex);
    ScopeBuilder_setDataMutex(self->builder, mutexes.dataMutex);
    ScopeBuilder_setTimestampReference(self->builder, scopeConfig.timestamp);
    ScopeBuilder_setCommunication(self->builder, FramedIO_getCommunicator(self->framedIO));
    ScopeBuilder_setAnnounceStorage(self->builder, self->announceStorage, scopeConfig.addressesInAddressAnnouncer);
    ScopeBuilder_setLogBuffer(self->builder, logConfig.logByteStream);

    /* Build the scope */
    self->scopeRunnable = ScopeBuilder_build(self->builder);

    return self;
}

// hier Funktion machen, die Stream generiert ScopeFramedStack_log..

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
