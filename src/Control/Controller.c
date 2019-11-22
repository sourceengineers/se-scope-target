/*!****************************************************************************************************************************************
 * @file         Controller.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include "Scope/GeneralPurpose/IRunnable.h"

#include "Scope/Control/CommandPackParserDispatcher.h"
#include "Scope/Control/Controller.h"
#include "Scope/Control/ICommand.h"
#include "Scope/Control/CommandParserDispatcher.h"
#include "Scope/Control/IPacker.h"
#include "Scope/Control/IUnpacker.h"
#include "Scope/Core/IScope.h"
#include "Scope/Core/ScopeTypes.h"
#include "Scope/GeneralPurpose/IObserver.h"
#include "Scope/Control/ParserDefinitions.h"

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

    bool packCommandPending;
    bool commandPending;
    PACK_TYPES typeToPack;

    uint8_t pendingToPack;

} ControllerPrivateData;

static bool runRx(IRunnableHandle runnable);

static bool runTx(IRunnableHandle runnable);

static void
fetchCommands(ControllerHandle self, IUnpackerHandle unpacker, ICommandHandle *commands, size_t numberOfCommands);

static void runCommands(ICommandHandle *commands, size_t numberOfCommands);

static void commandPackUpdate(IObserverHandle observer, void *state);

static void commandUpdate(IObserverHandle observer, void *state);

/******************************************************************************
 Private functions
******************************************************************************/
static bool runRx(IRunnableHandle runnable) {
    ControllerHandle self = (ControllerHandle) runnable->handle;

    if (self->commandPending == false) {
        return false;
    }

    if (self->unpacker == NULL) {
        return false;
    }

    size_t numberOfCommands = self->unpacker->getNumberOfCommands(self->unpacker);
    ICommandHandle commands[numberOfCommands];

    fetchCommands(self, self->unpacker, commands, numberOfCommands);
    runCommands(commands, numberOfCommands);
    self->commandPending = false;

    return true;
}

static bool runTx(IRunnableHandle runnable) {
    ControllerHandle self = (ControllerHandle) runnable->handle;

    if (self->packCommandPending == false) {
        return false;
    }

    if (self->packer == NULL){
        return false;
    }

    if (self->packer->isReady(self->packer) == false){
        return false;
    }

    ICommandHandle packCommand;

    if((self->pendingToPack & PACK_ANNOUNCE) != 0){
        packCommand = CommandPackParserDispatcher_run(self->commandPackParserDispatcher,
                                                      (const char *) "ev_pack_announce");
        packCommand->run(packCommand);
    }
    if((self->pendingToPack & PACK_DATA) != 0){
        packCommand = CommandPackParserDispatcher_run(self->commandPackParserDispatcher,
                                                      (const char *) "ev_pack_data");
        packCommand->run(packCommand);
    }
    if((self->pendingToPack & PACK_DETECT) != 0){
        packCommand = CommandPackParserDispatcher_run(self->commandPackParserDispatcher,
                                                      (const char *) "ev_pack_detect");
        packCommand->run(packCommand);
    }

    self->packCommandPending = false;
    self->pendingToPack = 0;
    self->packObserver->update(self->packObserver, NULL);

    return true;
}

static void
fetchCommands(ControllerHandle self, IUnpackerHandle unpacker, ICommandHandle *commands, size_t numberOfCommands) {

    char commandName[MAX_COMMAND_LENGTH];

    for (size_t i = 0; i < numberOfCommands; ++i) {
        unpacker->getNameOfCommand(unpacker, commandName, MAX_COMMAND_LENGTH, i);
        commands[i] = CommandParserDispatcher_run(self->commandParserDispatcher, commandName);
    }
}

static void runCommands(ICommandHandle *commands, size_t numberOfCommands) {
    for (size_t i = 0; i < numberOfCommands; ++i) {
        if (commands[i] != NULL) {
            commands[i]->run(commands[i]);
        }
    }
}

static void commandPackUpdate(IObserverHandle observer, void *state) {
    ControllerHandle self = (ControllerHandle) observer->handle;
    self->packCommandPending = true;
    self->pendingToPack |= *(uint8_t*) state;
}

static void commandUpdate(IObserverHandle observer, void *state) {
    ControllerHandle self = (ControllerHandle) observer->handle;
    self->commandPending = true;
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

    self->packCommandPending = false;
    self->commandPending = false;

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
