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

    IUnpackerHandle unpacker;
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
}

static void runTx(IRunnableHandle runnable){
    ControllerHandle self = (ControllerHandle) runnable->handle;
}

static void
fetchCommands(ControllerHandle self, IUnpackerHandle unpacker, ICommandHandle* commands, size_t numberOfCommands){

    /* Check if new data is pending */
    if(!self->unpacker->dataPending(self->unpacker)){
        return;
    }

    char commandName[MAX_COMMAND_LENGTH];

    for(size_t i = 0; i < numberOfCommands; ++i){
        unpacker->getNameOfCommand(unpacker, commandName, MAX_COMMAND_LENGTH, i);
        commands[i] = CommandParserDispatcher_run(self->commandParserDispatcher, commandName);
    }

    /* Singal the unpacker that the data was interpreted */
    self->unpacker->dataRead(self->unpacker);
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

    self->rxRunnable.run = &runRx;
    self->txRunnable.run = &runTx;
    self->unpacker = unpacker;
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
