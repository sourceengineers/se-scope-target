/*!****************************************************************************************************************************************
 * @file         CommandPoll.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Command/CommandPoll.h>

const char* commandName = "ev_poll";

/* Define public data */
typedef struct __CommandPollPrivateData
{
  ICommand iCommand;
  IScopeHandle iScope;
  
} CommandPollPrivateData ;

static void run(ICommandHandle self){
  CommandPollHandle commandPoll = (CommandPollHandle) self->implementer;
  Scope_poll(commandPoll->iScope);
}

static void setCommandAttribute(ICommandHandle self, void* attr){
  return;
}

/* Public functions */
CommandPollHandle CommandPoll_create(IScopeHandle iScope){

  CommandPollHandle self = malloc(sizeof(CommandPollPrivateData));
  self->iScope = iScope;
  
  return self;
}

ICommandHandle CommandPoll_getICommand(CommandPollHandle self){
  return &self->iCommand;
}

const char* CommandPoll_getName(CommandPollHandle self){
  return commandName;
}


void CommandPoll_destroy(CommandPollHandle self){
  free(self);
}
