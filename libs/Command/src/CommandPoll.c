/*!****************************************************************************************************************************************
 * @file         CommandPoll.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Command/CommandPoll.h>

static const char* commandName = "ev_poll";

/* Define public data */
typedef struct __CommandPollPrivateData
{
  ICommand iCommand;
  IScopeHandle iScope;
  
} CommandPollPrivateData ;

static void run(ICommandHandle self){
  CommandPollHandle commandPoll = (CommandPollHandle) self->implementer;
  commandPoll->iScope->poll(commandPoll->iScope);
}

static void setCommandAttribute(ICommandHandle self, void* attr){
  return;
}

/* Public functions */
CommandPollHandle CommandPoll_create(IScopeHandle iScope){

  CommandPollHandle self = malloc(sizeof(CommandPollPrivateData));
  self->iScope = iScope;
  
  self->iCommand.implementer = self;
  self->iCommand.run = &run;
  self->iCommand.setCommandAttribute = &setCommandAttribute;
  
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
  self = NULL;
}
