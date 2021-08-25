/*!****************************************************************************************************************************************
 * @file         CommandClear.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include "Scope/Control/Commands/Command/CommandClear.h"
#include "Scope/Control/ICommand.h"
#include "Scope/Core/IScope.h"

#include <stdlib.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandClearPrivateData{
    ICommand command;

    IScopeHandle scope;
    IObserverHandle observer;

} CommandClearPrivateData;

static void run(ICommandHandle command);

/******************************************************************************
 Private functions
******************************************************************************/
static void run(ICommandHandle command){
    CommandClearHandle self = (CommandClearHandle) command->handle;
    self->scope->clear(self->scope);
    MessageType resp = SE_ACK;
    self->observer->update(self->observer, &resp);
}

/******************************************************************************
 Private functions
******************************************************************************/
CommandClearHandle CommandClear_create(IScopeHandle scope, IObserverHandle observer){

    CommandClearHandle self = malloc(sizeof(CommandClearPrivateData));
    assert(self);

    self->scope = scope;
    self->observer = observer;

    self->command.handle = self;
    self->command.run = &run;

    return self;
}

ICommandHandle CommandClear_getICommand(CommandClearHandle self){
    return &self->command;
}

void CommandClear_destroy(CommandClearHandle self){
    free(self);
    self = NULL;
}
