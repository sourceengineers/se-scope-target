/*!****************************************************************************************************************************************
 * @file         CommandPoll.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Control/Command/CommandPoll.h>
#include <Scope/Control/Command/ICommand.h>
#include <Scope/Core/IScope.h>

#include <stdlib.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandPollPrivateData{
    ICommand command;

    IScopeHandle scope;

} CommandPollPrivateData;

static void run(ICommandHandle command);

/******************************************************************************
 Private functions
******************************************************************************/
static void run(ICommandHandle command){
    CommandPollHandle self = (CommandPollHandle) command->handle;
    self->scope->poll(self->scope);
}

/******************************************************************************
 Public functions
******************************************************************************/
CommandPollHandle CommandPoll_create(IScopeHandle scope){

    CommandPollHandle self = malloc(sizeof(CommandPollPrivateData));
    self->scope = scope;

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
