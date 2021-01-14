/*!****************************************************************************************************************************************
 * @file         Controller.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
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
    MessageType eventQueue[NUM_CLIENT_TO_HOST_MESSAGES];
    uint32_t eventQueueReadPos;
    uint32_t eventQueueWritePos;

} ControllerPrivateData;

static bool runRx(IRunnableHandle runnable);
static bool runTx(IRunnableHandle runnable);
static void commandPackUpdate(IObserverHandle observer, void *state);
static bool messageNeedsToBeAcked(MessageType message) ;
static void commandUpdate(IObserverHandle observer, void *state);
static void eventQueueAppend(ControllerHandle self, MessageType message);
static bool eventQueueIsEventPending(ControllerHandle self);
static MessageType eventQueuePop(ControllerHandle self);

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

    if (self->commandPending == SE_ACK) {
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

    // Check if messages have to be packed
    if (!eventQueueIsEventPending(self)) {
        return false;
    }

    if (self->packer->isReady(self->packer) == false){
        return false;
    }

    if (self->waitForAck == true){
        return false;
    }

    // Pop the latest event once everything is ready
    MessageType event = eventQueuePop(self);

    ICommandHandle packCommand;

    packCommand = CommandPackParserDispatcher_run(self->commandPackParserDispatcher, event);
    if(packCommand != NULL){
        packCommand->run(packCommand);
    }

    self->packObserver->update(self->packObserver, &event);
    self->waitForAck = messageNeedsToBeAcked(event);

    return true;
}

static void commandPackUpdate(IObserverHandle observer, void *state) {
    ControllerHandle self = (ControllerHandle) observer->handle;
    eventQueueAppend(self, *(MessageType*) state);
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
    return message > ENUM_START_CLIENT_TO_HOST && message < ENUM_START_HOST_TO_CLIENT;
}

/**
 * When waiting for an ack, it might happen that events overlap each other. To make sure that they are not lost/forgotten,
 * they have to be buffered in a way. This is done in a ringbuffer like structure, where every event can only be
 * represented once. Saving the events more than once does not make sense, since everything is done statically,
 * we don't have a way to save the data to be sent N times anyways.
 * Since every event is only represented once, we do not have to deal with buffer overflows as long as the
 * buffer length = events available = NUM_CLIENT_TO_HOST_MESSAGES + ACK + NAK
 * In the future this can be extended with priorities
 * @param self
 * @param message
 */
static void eventQueueAppend(ControllerHandle self, MessageType message) {

    uint32_t size = NUM_CLIENT_TO_HOST_MESSAGES;

    (void ) size;
    for(size_t i = 0; i < NUM_CLIENT_TO_HOST_MESSAGES; ++i){
        // If the event is already registered in the queue, it does not have to be added. So we do nothing
        if (self->eventQueue[i] == message) {
            return;
        }
    }

    // Otherwise, it has to be added in the next write position
    self->eventQueue[self->eventQueueWritePos] = message;
    self->eventQueueWritePos += 1;
    self->eventQueueWritePos %= NUM_CLIENT_TO_HOST_MESSAGES;
}

static bool eventQueueIsEventPending(ControllerHandle self) {
    return self->eventQueue[self->eventQueueReadPos] != SE_NONE;
}

static MessageType eventQueuePop(ControllerHandle self) {

    // Fetch the latest event and check if its one that needs to be processed. If so pop it
    MessageType event = self->eventQueue[self->eventQueueReadPos];

    if (event != SE_NONE) {
        self->eventQueue[self->eventQueueReadPos] = SE_NONE;
        self->eventQueueReadPos += 1;
        self->eventQueueReadPos %= NUM_CLIENT_TO_HOST_MESSAGES;
    }

    return event;
}

/******************************************************************************
 Public functions
******************************************************************************/
ControllerHandle Controller_create(IScopeHandle scope, IPackerHandle packer, IUnpackerHandle unpacker,
                                   AnnounceStorageHandle announceStorage) {

    ControllerHandle self = malloc(sizeof(ControllerPrivateData));
    assert(self);

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

    self->commandParserDispatcher = CommandParserDispatcher_create(scope, &self->commandPackObserver, unpacker);
    self->commandPackParserDispatcher = CommandPackParserDispatcher_create(scope, announceStorage, packer);

    self->commandPending = SE_NONE;

    for(size_t i = 0; i < NUM_CLIENT_TO_HOST_MESSAGES; ++i){
        self->eventQueue[i] = SE_NONE;
    }
    self->eventQueueReadPos = 0;
    self->eventQueueWritePos = 0;

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
