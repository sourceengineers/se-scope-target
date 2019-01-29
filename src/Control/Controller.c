/*!****************************************************************************************************************************************
 * @file         Controller.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Control/Controller.h>
#include <stdlib.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __ControllerPrivateData{
    IRunnable rxRunnable;
    IRunnable txRunnable;

    IScopeHandle scope;

    IUnpackerHandle unpacker;
    IPackerHandle packer;
    CommandParserDispatcherHandle commandParserDispatcher;
} ControllerPrivateData;

/* Fetches all commands from the Parser */
static void
fetchCommands(ControllerHandle self, IUnpackerHandle unpacker, ICommandHandle* commands, size_t numberOfCommands);

/* Executes all fetched commands */
static void runCommands(ICommandHandle* commands, size_t numberOfCommands);

/******************************************************************************
 Private functions
******************************************************************************/
static void runRx(IRunnableHandle runnable){
    ControllerHandle self = (ControllerHandle) runnable->handle;

    if(self->unpacker->dataPending(self->unpacker) == false){
        return;
    }

    size_t numberOfCommands = self->unpacker->getNumberOfCommands(self->unpacker);
    ICommandHandle commands[numberOfCommands];

    fetchCommands(self, self->unpacker, commands, numberOfCommands);
    runCommands(commands, numberOfCommands);

    /* Singal the unpacker that the data was interpreted */
    self->unpacker->dataRead(self->unpacker);
}

static void runTx(IRunnableHandle runnable){
    ControllerHandle self = (ControllerHandle) runnable->handle;

    if((self->scope->scopeIsReadyToSend(self->scope) == false) && \
        self->packer->flowControlReadyToSend(self->packer) == false){
        return;
    }

    ICommandHandle packCommand;
    packCommand = CommandParserDispatcher_run(self->commandParserDispatcher, (const char*) "ev_pack");

    if(packCommand == NULL){
        return;
    }

    packCommand->run(packCommand);
}

static void
fetchCommands(ControllerHandle self, IUnpackerHandle unpacker, ICommandHandle* commands, size_t numberOfCommands){

    char commandName[MAX_COMMAND_LENGTH];

    for(size_t i = 0; i < numberOfCommands; ++i){
        unpacker->getNameOfCommand(unpacker, commandName, MAX_COMMAND_LENGTH, i);
        commands[i] = CommandParserDispatcher_run(self->commandParserDispatcher, commandName);
    }
}

static void runCommands(ICommandHandle* commands, size_t numberOfCommands){
    for(size_t i = 0; i < numberOfCommands; ++i){
        if(commands[i] != NULL){
            commands[i]->run(commands[i]);
        }
    }
}

/******************************************************************************
 Public functions
******************************************************************************/
ControllerHandle Controller_create(IScopeHandle scope, IPackerHandle packer, IUnpackerHandle unpacker){

    ControllerHandle self = malloc(sizeof(ControllerPrivateData));

    self->rxRunnable.handle = self;
    self->txRunnable.handle = self;
    self->rxRunnable.run = &runRx;
    self->txRunnable.run = &runTx;
    self->unpacker = unpacker;
    self->packer = packer;
    self->scope = scope;
    self->commandParserDispatcher = CommandParserDispatcher_create(scope, packer, unpacker);

    return self;
}

IRunnableHandle Controller_getRxRunnable(ControllerHandle self){
    return &self->rxRunnable;
}

IRunnableHandle Controller_getTxRunnable(ControllerHandle self){
    return &self->txRunnable;
}

void Controller_destroy(ControllerHandle self){
    CommandParserDispatcher_destroy(self->commandParserDispatcher);

    free(self);
    self = NULL;
}
