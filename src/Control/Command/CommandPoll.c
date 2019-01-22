/*!****************************************************************************************************************************************
 * @file         CommandPoll.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Control/Command/CommandPoll.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandPollPrivateData{
    ICommand command;
    IScopeHandle scope;

    uint32_t nextTimeStamp;

} CommandPollPrivateData;

/******************************************************************************
 Private functions
******************************************************************************/
static void run(ICommandHandle command){
  CommandPollHandle self = (CommandPollHandle) command->handle;
  self->scope->poll(self->scope, self->nextTimeStamp);
}

/******************************************************************************
 Public functions
******************************************************************************/
CommandPollHandle CommandPoll_create(IScopeHandle scope){

  CommandPollHandle self = malloc(sizeof(CommandPollPrivateData));
  self->scope = scope;

  self->command.handle = self;
  self->command.run = &run;

  self->nextTimeStamp = 0;

  return self;
}

ICommandHandle CommandPoll_getICommand(CommandPollHandle self){
  return &self->command;
}

void CommandPoll_setAttributes(CommandPollHandle self, uint32_t timestamp){
  self->nextTimeStamp = timestamp;
}

void CommandPoll_destroy(CommandPollHandle self){
  free(self);
  self = NULL;
}
