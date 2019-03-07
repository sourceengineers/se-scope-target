/*!****************************************************************************************************************************************
 * @file         Controller.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Control/CommandPackParserDispatcher.h>
#include <Scope/Control/Controller.h>
#include <Scope/Control/Command/ICommand.h>
#include <Scope/Control/CommandParserDispatcher.h>
#include <Scope/Control/IPacker.h>
#include <Scope/Control/IUnpacker.h>
#include <Scope/Core/IScope.h>
#include <Scope/Core/ScopeTypes.h>
#include <Scope/GeneralPurpose/IObserver.h>
#include <Scope/GeneralPurpose/IRunnable.h>

#include <stdbool.h>
#include <stdlib.h>

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

} ControllerPrivateData;

static void runRx(IRunnableHandle runnable);

static void runTx(IRunnableHandle runnable);

static void
fetchCommands(ControllerHandle self, IUnpackerHandle unpacker, ICommandHandle *commands, size_t numberOfCommands);

static void runCommands(ICommandHandle *commands, size_t numberOfCommands);

static void commandPackUpdate(IObserverHandle observer, void *state);

static void commandUpdate(IObserverHandle observer, void *state);

/******************************************************************************
 Private functions
******************************************************************************/
static void runRx(IRunnableHandle runnable) {
    ControllerHandle self = (ControllerHandle) runnable->handle;

    if (self->commandPending == false) {
        return;
    }

    if (self->unpacker == NULL) {
        return;
    }

    size_t numberOfCommands = self->unpacker->getNumberOfCommands(self->unpacker);
    ICommandHandle commands[numberOfCommands];

    fetchCommands(self, self->unpacker, commands, numberOfCommands);
    runCommands(commands, numberOfCommands);
    self->commandPending = false;
}

static void runTx(IRunnableHandle runnable) {
    ControllerHandle self = (ControllerHandle) runnable->handle;

    if (self->packCommandPending == false) {
        return;
    }

    ICommandHandle packCommand;

    switch (self->typeToPack) {

        case PACK_ANNOUNCE:
            packCommand = CommandPackParserDispatcher_run(self->commandPackParserDispatcher,
                                                          (const char *) "ev_pack_announce");
            break;
        case PACK_DATA:
            packCommand = CommandPackParserDispatcher_run(self->commandPackParserDispatcher,
                                                          (const char *) "ev_pack_data");
            break;
        default:
            packCommand = NULL;
            break;
    }

    if (packCommand == NULL) {
        return;
    }

    packCommand->run(packCommand);
    self->packCommandPending = false;
    self->packObserver->update(self->packObserver, NULL);
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
    self->typeToPack = *(PACK_TYPES *) state;
}

static void commandUpdate(IObserverHandle observer, void *state) {
    ControllerHandle self = (ControllerHandle) observer->handle;
    self->commandPending = true;
}

/******************************************************************************
 Public functions
******************************************************************************/
ControllerHandle Controller_create(IScopeHandle scope, IPackerHandle packer, IUnpackerHandle unpacker) {

    ControllerHandle self = malloc(sizeof(ControllerPrivateData));

    self->rxRunnable.handle = self;
    self->txRunnable.handle = self;
    self->rxRunnable.run = &runRx;
    self->txRunnable.run = &runTx;
    self->unpacker = unpacker;
    self->scope = scope;

    self->commandPackObserver.handle = self;
    self->commandObserver.handle = self;

    self->commandPackObserver.update = &commandPackUpdate;
    self->commandObserver.update = &commandUpdate;

    self->commandParserDispatcher = CommandParserDispatcher_create(scope, unpacker);
    self->commandPackParserDispatcher = CommandPackParserDispatcher_create(scope, packer);

    self->packCommandPending = false;
    self->commandPending = false;

    return self;
}


void Controller_attachPackObserver(ControllerHandle self, IObserverHandle observer) {
    self->packObserver = observer;
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
