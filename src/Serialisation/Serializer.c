#include <limits.h>
/*!****************************************************************************************************************************************
 * @file         Serializer.c
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

#include <se-lib-c/util/runnable/IRunnable.h>
#include <se-lib-c/util/observer/IObserver.h>

#include "Scope/Control/ParserDefinitions.h"
#include "Scope/Serialisation/Serializer.h"
#include "Scope/Control/IPacker.h"
#include "Scope/Control/IUnpacker.h"

#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <Scope/Serialisation/Protobuf/NanopbPacker.h>
#include <Scope/Serialisation/Protobuf/NanopbUnpacker.h>


/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __SerializerPrivateData{
    IRunnable runRx;
    IRunnable runTx;
    IPacker packer;
    IUnpacker unpacker;

    MessageType unpackingPending;
    MessageType packingPending;

    IObserver packObserver;
    IObserver unpackObserver;
    NanopbPackerHandle nanopbPacker;
    NanopbUnpackerHandle nanopbUnpacker;
    IByteStreamHandle output;
    IByteStreamHandle input;

    IObserverHandle controlObserver;
    IObserverHandle communicationObserver;
} SerializerPrivateData;

static bool runRx(IRunnableHandle runnable);

static bool runTx(IRunnableHandle runnable);

static void updateUnpacker(IObserverHandle observer, void* state);

static void updatePacker(IObserverHandle observer, void* state);

static IPackerHandle getPacker(SerializerHandle self, MessageType type);

static IUnpackerHandle getUnpacker(SerializerHandle self, MessageType type);

static bool unpack(IUnpackerHandle unpacker, MessageType type);

static uint32_t evPoll_getTimestamp(IUnpackerHandle unpacker);

static size_t cfRunning_getAmount(IUnpackerHandle unpacker);

static CfRunningDef cfRunning_getChannel(IUnpackerHandle unpacker, uint32_t index);

static CfTriggerDef cfTrigger_getTriggerConfig(IUnpackerHandle unpacker);

static uint32_t cfTInc_getInc(IUnpackerHandle unpacker);

static uint32_t cfAddress_getAmount(IUnpackerHandle unpacker);

static CfAddressDef cfAddress_getChannel(IUnpackerHandle unpacker, uint32_t index);

static void addChannel(IPackerHandle packer, ScDataChannelDef channel);

static void addLog(IPackerHandle packer, ScLogDataDef logStream);

static void addTimeIncrement(IPackerHandle packer, const uint32_t timeIncrement);

static void addTrigger(IPackerHandle packer, ScDataTriggerDef trigger);

static void addTimestamp(IPackerHandle packer, IIntStreamHandle timestamp);

static void addAddressAnnouncement(IPackerHandle packer, ScAnnounceChannelDef address);

static void addAnnouncement(IPackerHandle packer, const ScAnnounceMetaData meta);

static void reset(IPackerHandle packer);

/******************************************************************************
 Private functions
******************************************************************************/
inline static IPackerHandle getPacker(SerializerHandle self, MessageType type){
    (void)(type);
    return NanopbPacker_getIPacker(self->nanopbPacker);
}

inline static IUnpackerHandle getUnpacker(SerializerHandle self, MessageType type){
    if(type == EV_POLL ||
       type == CF_ADDR ||
       type == CF_RUNNING ||
       type == CF_T_INC ||
       type == CF_TRIGGER)
    {
        return NanopbUnpacker_getIUnpacker(self->nanopbUnpacker);
    } else {
        return NULL;
    }
}

static bool runRx(IRunnableHandle runnable){
    SerializerHandle self = (SerializerHandle) runnable->handle;

    if(self->unpackingPending == SE_NONE){
        return false;
    }

    IUnpackerHandle unpacker = getUnpacker(self, self->unpackingPending);
    bool parsingIsValid = true;

    if(unpacker != NULL){
        parsingIsValid = unpacker->unpack(unpacker, self->unpackingPending);
    }

    if(parsingIsValid){
        self->controlObserver->update(self->controlObserver, &self->unpackingPending);
    }else{
        MessageType type = SE_NAK;
        self->controlObserver->update(self->controlObserver, &type);
    }

    self->unpackingPending = SE_NONE;

    return true;
}

static bool runTx(IRunnableHandle runnable){
    SerializerHandle self = (SerializerHandle) runnable->handle;

    if(self->packingPending == SE_NONE){
        return false;
    }

    self->packer.pack(&self->packer, self->packingPending);
    self->communicationObserver->update(self->communicationObserver, &self->packingPending);
    self->packingPending = SE_NONE;

    return true;
}

static void updateUnpacker(IObserverHandle observer, void* state){
    SerializerHandle self = (SerializerHandle) observer->handle;
    self->unpackingPending = *(MessageType*) state;
}

static void updatePacker(IObserverHandle observer, void* state){
    SerializerHandle self = (SerializerHandle) observer->handle;
    self->packingPending = *(MessageType*) state;
}

void pack(IPackerHandle packer, MessageType type){
    SerializerHandle self = (SerializerHandle) packer->handle;
    IPackerHandle currentPacker = getPacker(self, type);

    if(currentPacker == NULL){
        return;
    }

    currentPacker->pack(currentPacker, type);
}

bool isReady(IPackerHandle packer){
    SerializerHandle self = (SerializerHandle) packer->handle;
    return self->packingPending == SE_NONE && self->output->length(self->output) == 0;
}

void addChannel(IPackerHandle packer, ScDataChannelDef channel){
    SerializerHandle self = (SerializerHandle) packer->handle;
    IPackerHandle currentPacker = getPacker(self, SC_DATA);
    currentPacker->addChannel(currentPacker, channel);
}

void addLog(IPackerHandle packer, ScLogDataDef logStream){
    SerializerHandle self = (SerializerHandle) packer->handle;
    IPackerHandle currentPacker = getPacker(self, SC_LOG);
    currentPacker->addLog(currentPacker, logStream);
}

void addTimeIncrement(IPackerHandle packer, const uint32_t timeIncrement){
    SerializerHandle self = (SerializerHandle) packer->handle;
    IPackerHandle currentPacker = getPacker(self, SC_DATA);
    currentPacker->addTimeIncrement(currentPacker, timeIncrement);
}

void addTimestamp(IPackerHandle packer, IIntStreamHandle timestamp){
    SerializerHandle self = (SerializerHandle) packer->handle;
    IPackerHandle currentPacker = getPacker(self, SC_DATA);
    currentPacker->addTimestamp(currentPacker, timestamp);
}

void addTrigger(IPackerHandle packer, ScDataTriggerDef trigger){
    SerializerHandle self = (SerializerHandle) packer->handle;
    IPackerHandle currentPacker = getPacker(self, SC_DATA);
    currentPacker->addTrigger(currentPacker, trigger);
}

void addAddressAnnouncement(IPackerHandle packer, ScAnnounceChannelDef address){
    SerializerHandle self = (SerializerHandle) packer->handle;
    IPackerHandle currentPacker = getPacker(self, SC_ANNOUNCE);
    currentPacker->addAddressAnnouncement(currentPacker, address);
}

void addAnnouncement(IPackerHandle packer, const ScAnnounceMetaData meta){
    SerializerHandle self = (SerializerHandle) packer->handle;
    IPackerHandle currentPacker = getPacker(self, SC_ANNOUNCE);
    currentPacker->addAnnouncement(currentPacker, meta);
}

static bool unpack(IUnpackerHandle unpacker, MessageType type){
    SerializerHandle self = (SerializerHandle) unpacker->handle;
    IUnpackerHandle currentUnpacker = getUnpacker(self, type);
    if(currentUnpacker == NULL){
        return NULL;
    }
    return currentUnpacker->unpack(currentUnpacker, type);
}

static uint32_t evPoll_getTimestamp(IUnpackerHandle unpacker){
    SerializerHandle self = (SerializerHandle) unpacker->handle;
    IUnpackerHandle currentUnpacker = getUnpacker(self, EV_POLL);
    return currentUnpacker->evPoll_getTimestamp(currentUnpacker);
}

static size_t cfRunning_getAmount(IUnpackerHandle unpacker){
    SerializerHandle self = (SerializerHandle) unpacker->handle;
    IUnpackerHandle currentUnpacker = getUnpacker(self, CF_RUNNING);
    return currentUnpacker->cfRunning_getAmount(currentUnpacker);
}

static CfRunningDef cfRunning_getChannel(IUnpackerHandle unpacker, uint32_t index){
    SerializerHandle self = (SerializerHandle) unpacker->handle;
    IUnpackerHandle currentUnpacker = getUnpacker(self, CF_RUNNING);
    return currentUnpacker->cfRunning_getChannel(currentUnpacker, index);
}

static CfTriggerDef cfTrigger_getTriggerConfig(IUnpackerHandle unpacker){
    SerializerHandle self = (SerializerHandle) unpacker->handle;
    IUnpackerHandle currentUnpacker = getUnpacker(self, CF_TRIGGER);
    return currentUnpacker->cfTrigger_getTriggerConfig(currentUnpacker);
}

static uint32_t cfTInc_getInc(IUnpackerHandle unpacker){
    SerializerHandle self = (SerializerHandle) unpacker->handle;
    IUnpackerHandle currentUnpacker = getUnpacker(self, CF_RUNNING);
    return currentUnpacker->cfTInc_getInc(currentUnpacker);
}

static uint32_t cfAddress_getAmount(IUnpackerHandle unpacker){
    SerializerHandle self = (SerializerHandle) unpacker->handle;
    IUnpackerHandle currentUnpacker = getUnpacker(self, CF_ADDR);
    return currentUnpacker->cfAddress_getAmount(currentUnpacker);
}

static CfAddressDef cfAddress_getChannel(IUnpackerHandle unpacker, uint32_t index){
    SerializerHandle self = (SerializerHandle) unpacker->handle;
    IUnpackerHandle currentUnpacker = getUnpacker(self, CF_ADDR);
    return currentUnpacker->cfAddress_getChannel(currentUnpacker, index);
}

void reset(IPackerHandle packer){
    (void)(packer);
}

/******************************************************************************
 Public functions
******************************************************************************/
SerializerHandle Serializer_create(size_t maxChannels, size_t maxAddresses, IByteStreamHandle output,
        IByteStreamHandle input){

    SerializerHandle self = malloc(sizeof(SerializerPrivateData));
    assert(self);

    self->nanopbPacker = NanopbPacker_create(maxChannels, maxAddresses, output);

    self->nanopbUnpacker = NanopbUnpacker_create(input, maxChannels);

    self->packObserver.handle = self;
    self->unpackObserver.handle = self;
    self->packObserver.update = &updatePacker;
    self->unpackObserver.update = &updateUnpacker;
    self->runRx.handle = self;
    self->runTx.handle = self;
    self->runRx.run = &runRx;
    self->runTx.run = &runTx;
    self->output = output;
    self->input = input;

    self->packer.handle = self;
    self->packer.addTimeIncrement = &addTimeIncrement;
    self->packer.addTrigger = &addTrigger;
    self->packer.reset = &reset;
    self->packer.pack = &pack;
    self->packer.addAnnouncement = &addAnnouncement;
    self->packer.isReady = &isReady;
    self->packer.addChannel = &addChannel;
    self->packer.addAddressAnnouncement = &addAddressAnnouncement;
    self->packer.addTimestamp = &addTimestamp;
    self->packer.addLog = &addLog;

    self->unpacker.handle = self;
    self->unpacker.cfAddress_getAmount = &cfAddress_getAmount;
    self->unpacker.cfAddress_getChannel = &cfAddress_getChannel;
    self->unpacker.cfRunning_getAmount = &cfRunning_getAmount;
    self->unpacker.cfRunning_getChannel = &cfRunning_getChannel;
    self->unpacker.cfTInc_getInc = &cfTInc_getInc;
    self->unpacker.cfTrigger_getTriggerConfig = &cfTrigger_getTriggerConfig;
    self->unpacker.evPoll_getTimestamp = &evPoll_getTimestamp;
    self->unpacker.unpack = &unpack;

    self->unpackingPending = SE_NONE;
    self->packingPending = SE_NONE;
    return self;
}

IRunnableHandle Serializer_getTxRunnable(SerializerHandle self){
    return &self->runTx;
}

IRunnableHandle Serializer_getRxRunnable(SerializerHandle self){
    return &self->runRx;
}

void Serializer_attachCommunicationObserver(SerializerHandle self, IObserverHandle observer){
    self->communicationObserver = observer;
}

void Serializer_attachControlObserver(SerializerHandle self, IObserverHandle observer){
    self->controlObserver = observer;
}

IObserverHandle Serializer_getPackObserver(SerializerHandle self){
    return &self->packObserver;
}

IObserverHandle Serializer_getUnpackObserver(SerializerHandle self){
    return &self->unpackObserver;
}

IPackerHandle Serializer_getPacker(SerializerHandle self){
    return &self->packer;
}

IUnpackerHandle Serializer_getUnpacker(SerializerHandle self){
    return &self->unpacker;
}

size_t
Serializer_txCalculateBufferSize(size_t amountOfChannels, size_t sizeOfChannels, size_t addressesInAddressAnnouncer){
    size_t nanopbPackerSize = NanopbPacker_calculateBufferSize(amountOfChannels, sizeOfChannels,
                                                               addressesInAddressAnnouncer);
    return nanopbPackerSize;
}

size_t Serializer_rxCalculateBufferSize(size_t maxChannels){
    return NanopbUnpacker_calculateBufferSize(maxChannels);
}

void Serializer_destroy(SerializerHandle self){
    NanopbUnpacker_destroy(self->nanopbUnpacker);
    NanopbPacker_destroy(self->nanopbPacker);
    free(self);
    self = NULL;
}
