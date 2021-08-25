/*!****************************************************************************************************************************************
 * @file         CommandPoll.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include "Scope/Control/Commands/Command/CommandPoll.h"
#include "Scope/Control/ICommand.h"
#include "Scope/Core/IScope.h"

#include <stdlib.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandPollPrivateData{
    ICommand command;

    IScopeHandle scope;
    IObserverHandle observer;

} CommandPollPrivateData;

static void run(ICommandHandle command);

/******************************************************************************
 Private functions
******************************************************************************/
static void run(ICommandHandle command){
    CommandPollHandle self = (CommandPollHandle) command->handle;
    self->scope->poll(self->scope);
    MessageType resp = SE_ACK;
    self->observer->update(self->observer, &resp);
}

/******************************************************************************
 Public functions
******************************************************************************/
CommandPollHandle CommandPoll_create(IScopeHandle scope, IObserverHandle observer){

    CommandPollHandle self = malloc(sizeof(CommandPollPrivateData));
    assert(self);

    self->scope = scope;

    self->observer = observer;
    self->command.handle = self;
    self->command.run = &run;

    return self;
}

ICommandHandle CommandPoll_getICommand(CommandPollHandle self){
    return &self->command;
}

void CommandPoll_destroy(CommandPollHandle self){
    free(self);
    self = NULL;
}
