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
    MessageType packCommandPending;
    MessageType commandPending;

    bool waitForAck;
} ControllerPrivateData;

static bool runRx(IRunnableHandle runnable);
static bool runTx(IRunnableHandle runnable);
static void commandPackUpdate(IObserverHandle observer, void *state);
static bool messageNeedsToBeAcked(MessageType message) ;
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
    if (self->packCommandPending == SE_NONE) {
        return false;
    }

    if (self->packer->isReady(self->packer) == false){
        return false;
    }

    if (self->waitForAck == true){
        return false;
    }

    ICommandHandle packCommand;

    packCommand = CommandPackParserDispatcher_run(self->commandPackParserDispatcher, self->packCommandPending);
    if(packCommand != NULL){
        packCommand->run(packCommand);
    }

    self->packObserver->update(self->packObserver, &self->packCommandPending);
    self->waitForAck = messageNeedsToBeAcked(self->packCommandPending);
    self->packCommandPending = SE_NONE;
		
    return true;
}

static void commandPackUpdate(IObserverHandle observer, void *state) {
    ControllerHandle self = (ControllerHandle) observer->handle;
    if(self->packCommandPending != SE_NONE){
        return;
    }
    self->packCommandPending = *(MessageType*) state;
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

    self->commandPackObserver.handle = self;
    self->commandObserver.handle = self;

    self->commandPackObserver.update = &commandPackUpdate;
    self->commandObserver.update = &commandUpdate;

    self->commandParserDispatcher = CommandParserDispatcher_create(scope, &self->commandPackObserver, unpacker);
    self->commandPackParserDispatcher = CommandPackParserDispatcher_create(scope, announceStorage, packer);

    self->commandPending = SE_NONE;
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
