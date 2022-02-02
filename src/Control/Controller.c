/*!****************************************************************************************************************************************
 * @file         Controller.c
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

#include "Scope/Control/CommandPackParserDispatcher.h"
#include "Scope/Control/Controller.h"
#include "Scope/Control/ICommand.h"
#include "Scope/Control/CommandParserDispatcher.h"
#include "Scope/Control/IPacker.h"
#include "Scope/Control/IUnpacker.h"
#include "Scope/Core/IScope.h"
#include "Scope/Core/ScopeTypes.h"
#include "se-lib-c/algorithm/AgingPriorityQueue.h"

#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __ControllerPrivateData {
    IRunnable rxRunnable;
    IRunnable txRunnable;
    IScopeHandle scope;
    IUnpackerHandle unpacker;
    IPackerHandle packer;
    CommandParserDispatcherHandle commandParserDispatcher;
    CommandPackParserDispatcherHandle commandPackParserDispatcher;
    IObserver commandObserver;
    IObserver commandPackObserver;
    IObserverHandle packObserver;
    MessageType commandPending;
    bool waitForAck;
    AgingPriorityQueueHandle queue;
    Message_Priorities priorities;
    IByteStreamHandle logByteStream;
} ControllerPrivateData;

static bool runRx(IRunnableHandle runnable);
static bool runTx(IRunnableHandle runnable);
static void commandPackUpdate(IObserverHandle observer, void *state);
static void commandPackPushEvent(ControllerHandle self, MessageType event);
static bool messageNeedsToBeAcked(MessageType message);
static bool messageCanUnlockAckLock(MessageType message);
static void commandUpdate(IObserverHandle observer, void *state);

/******************************************************************************
 Private functions
******************************************************************************/
static bool runRx(IRunnableHandle runnable) {
    ControllerHandle self = (ControllerHandle) runnable->handle;

    if (self->commandPending == SE_NONE) {
        return false;
    }

    if (self->commandPending == SE_NAK) {
        self->commandPackObserver.update(&self->commandPackObserver, &self->commandPending);
        self->commandPending = SE_NONE;
        return true;
    }

    if (messageCanUnlockAckLock(self->commandPending)) {
        self->waitForAck = false;
    }

    ICommandHandle command = CommandParserDispatcher_run(self->commandParserDispatcher, self->commandPending);
    if(command != NULL){
        command->run(command);
    }

    self->commandPending = SE_NONE;

    return true;
}

static bool runTx(IRunnableHandle runnable) {
    ControllerHandle self = (ControllerHandle) runnable->handle;

    if (self->packer->isReady(self->packer) == false){
        return false;
    }

    if (self->waitForAck == true){
        return false;
    }

    if (self->logByteStream->length(self->logByteStream->handle) > 0) {
        commandPackPushEvent(self, SC_LOG);
    }

    // Check if messages have to be packed
    if (AgingPriorityQueue_empty(self->queue)) {
        return false;
    }

    // Pop the latest event once everything is ready
    uint32_t item;
    int ret = AgingPriorityQueue_pop(self->queue, &item);
    MessageType event = (MessageType) item;

    if (ret < 0) {
        return false;
    }

    ICommandHandle packCommand = CommandPackParserDispatcher_run(self->commandPackParserDispatcher, event);
    if(packCommand != NULL){
        packCommand->run(packCommand);
    }

    self->packObserver->update(self->packObserver, &event);
    self->waitForAck = messageNeedsToBeAcked(event);

    return true;
}

static void commandPackPushEvent(ControllerHandle self, MessageType event) {
    switch (event) {
        case SE_NAK:
            AgingPriorityQueue_push(self->queue, SE_NAK, 0);
            break;
        case SE_ACK:
            AgingPriorityQueue_push(self->queue, SE_ACK, 0);
            break;
        case SC_DATA:
            {
            uint32_t prio = (uint32_t) self->priorities.data;
            AgingPriorityQueue_push(self->queue, SC_DATA, prio);
            }
            break;
        case SC_ANNOUNCE:
            AgingPriorityQueue_push(self->queue, SC_ANNOUNCE, 1);
            break;
        case SC_DETECT:
            AgingPriorityQueue_push(self->queue, SC_DETECT, 1);
            break;
        case SC_STREAM:
            {
            uint32_t prio = (uint32_t) self->priorities.stream;
            AgingPriorityQueue_push(self->queue, SC_STREAM, prio);
            }
            break;
        case SC_LOG:
            // Only append the sc log event if it doesn't already exist to avoid flooding the queue with messages.
            // This is necessary because the log is polled and not event driven.
            if (!AgingPriorityQueue_contains(self->queue, SC_LOG)) {
                uint32_t prio = self->priorities.log;
                AgingPriorityQueue_push(self->queue, SC_LOG, prio);
            }
            break;
        default:
            break;
    }
}

static void commandPackUpdate(IObserverHandle observer, void *state) {
    ControllerHandle self = (ControllerHandle) observer->handle;
    commandPackPushEvent(self, *(MessageType*) state);
}

static void commandUpdate(IObserverHandle observer, void *state) {
    ControllerHandle self = (ControllerHandle) observer->handle;
    self->commandPending = *(MessageType*) state;
}

/**
 * All messages other than ack and nack, must be acked
 * @param message
 */
static bool messageNeedsToBeAcked(MessageType message) {
    return message == SC_LOG || message == SC_DATA || message == SC_STREAM;
//    return message > ENUM_START_CLIENT_TO_HOST && message < ENUM_START_HOST_TO_CLIENT;
}

static bool messageCanUnlockAckLock(MessageType message) {
    return message >= ENUM_START_HOST_TO_CLIENT || message == SE_NAK || message == SE_ACK;
}

/******************************************************************************
 Public functions
******************************************************************************/
ControllerHandle Controller_create(IScopeHandle scope,
                                   IPackerHandle packer,
                                   IUnpackerHandle unpacker,
                                   AnnounceStorageHandle announceStorage,
                                   IByteStreamHandle logByteStream,
                                   Message_Priorities priorities,
                                   size_t amountOfChannels) {

    ControllerHandle self = malloc(sizeof(ControllerPrivateData));
    assert(self);
    assert(logByteStream != NULL);

    self->rxRunnable.handle = self;
    self->txRunnable.handle = self;
    self->rxRunnable.run = &runRx;
    self->txRunnable.run = &runTx;
    self->unpacker = unpacker;
    self->packer = packer;
    self->scope = scope;
    self->waitForAck = false;
    self->commandPackObserver.handle = self;
    self->commandObserver.handle = self;

    self->commandPackObserver.update = &commandPackUpdate;
    self->commandObserver.update = &commandUpdate;

    self->priorities = priorities;

    self->logByteStream = logByteStream;

    self->commandParserDispatcher = CommandParserDispatcher_create(scope, &self->commandPackObserver, unpacker, amountOfChannels);

    self->commandPackParserDispatcher = CommandPackParserDispatcher_create(scope, announceStorage, packer, logByteStream);

    self->commandPending = SE_NONE;

    self->queue = AgingPriorityQueue_create(LOW + 1, 10, 3);

    self->waitForAck = true;

    return self;
}

void Controller_attachPackObserver(ControllerHandle self, IObserverHandle observer) {
    self->packObserver = observer;
}

bool Controller_commandPending(ControllerHandle self){
    return self->commandPending;
}

IObserverHandle Controller_getCommandObserver(ControllerHandle self) {
    return &self->commandObserver;
}

IObserverHandle Controller_getCommandPackObserver(ControllerHandle self) {
    return &self->commandPackObserver;
}

IRunnableHandle Controller_getRxRunnable(ControllerHandle self) {
    return &self->rxRunnable;
}

IRunnableHandle Controller_getTxRunnable(ControllerHandle self) {
    return &self->txRunnable;
}

void Controller_destroy(ControllerHandle self) {
    CommandParserDispatcher_destroy(self->commandParserDispatcher);
    CommandPackParserDispatcher_destroy(self->commandPackParserDispatcher);

    free(self);
    self = NULL;
}
