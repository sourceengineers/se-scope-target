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


#define MAX_COMMANDS_PEDNING_TO_PACK 5

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

    MESSAGE_TYPE packCommandPending[MAX_COMMANDS_PEDNING_TO_PACK];
    size_t messagesPendingToPack;

    MESSAGE_TYPE commandPending;


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

    if (self->commandPending == SE_NONE) {
        return false;
    }

    ICommandHandle command = CommandParserDispatcher_run(self->commandParserDispatcher, self->commandPending);
    command->run(command);

//    size_t numberOfCommands = self->unpacker->getNumberOfCommands(self->unpacker);
//    ICommandHandle commands[numberOfCommands];
//    fetchCommands(self, self->unpacker, commands, numberOfCommands);
//    runCommands(commands, numberOfCommands);
    self->commandPending = SE_NONE;

    return true;
}

static bool runTx(IRunnableHandle runnable) {
    ControllerHandle self = (ControllerHandle) runnable->handle;

    if (self->packer->isReady(self->packer) == false){
        return false;
    }

    // Check if messages have to be packed
    if (self->messagesPendingToPack <= 0) {
        return false;
    }

    // If so, decrease the count and fetch the message
    self->messagesPendingToPack -= 1;
    MESSAGE_TYPE type = self->packCommandPending[self->messagesPendingToPack];

    if(type == SE_NONE){
        return false;
    }

    ICommandHandle packCommand;

    packCommand = CommandPackParserDispatcher_run(self->commandPackParserDispatcher, type);
    packCommand->run(packCommand);

    self->packObserver->update(self->packObserver, &type);

    return true;
}

// static void
// fetchCommands(ControllerHandle self, IUnpackerHandle unpacker, ICommandHandle *commands, size_t numberOfCommands) {
//
//     char commandName[MAX_COMMAND_LENGTH];
//
//     for (size_t i = 0; i < numberOfCommands; ++i) {
//         unpacker->getNameOfCommand(unpacker, commandName, MAX_COMMAND_LENGTH, i);
//         commands[i] = CommandParserDispatcher_run(self->commandParserDispatcher, commandName);
//     }
// }
//
// static void runCommands(ICommandHandle *commands, size_t numberOfCommands) {
//     for (size_t i = 0; i < numberOfCommands; ++i) {
//         if (commands[i] != NULL) {
//             commands[i]->run(commands[i]);
//         }
//     }
// }

static void commandPackUpdate(IObserverHandle observer, void *state) {
    ControllerHandle self = (ControllerHandle) observer->handle;
    if(self->messagesPendingToPack < MAX_COMMANDS_PEDNING_TO_PACK){
        self->packCommandPending[self->messagesPendingToPack] = *(MESSAGE_TYPE*) state;
        self->messagesPendingToPack += 1;
    }
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

    self->commandPending = false;
    self->messagesPendingToPack = 0;

    for(int i = 0; i < MAX_COMMANDS_PEDNING_TO_PACK; ++i){
        self->packCommandPending[i] = SE_NONE;
    }

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
