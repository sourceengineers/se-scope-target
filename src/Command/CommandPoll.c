/*!****************************************************************************************************************************************
 * @file         CommandPoll.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Command/CommandPoll.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Name of the command */
static char* commandName = "ev_poll";

/* Class data */
typedef struct __CommandPollPrivateData
{
  ICommand command;
  IScopeHandle scope;

  uint32_t nextTimeStamp;

} CommandPollPrivateData ;

/* Implementation of the run command, which will be passed into the interface */
static void run(ICommandHandle command);

/* Implementation of the getCommandName command, which will be passed into the interface */
static char* getCommandName(ICommandHandle command);

/* Implementation of the setCommandAttribute command, which will be passed into the interface */
static void setCommandAttribute(ICommandHandle command, void* attr);

/******************************************************************************
 Private functions
******************************************************************************/
static void run(ICommandHandle command){
  CommandPollHandle self = (CommandPollHandle) command->implementer;
  self->scope->poll(self->scope, self->nextTimeStamp);
}

static void setCommandAttribute(ICommandHandle command, void* attr){
  CommandPollHandle self = (CommandPollHandle) command->implementer;

  self->nextTimeStamp = *(uint32_t*) attr;
}

static char* getCommandName(ICommandHandle command){
  CommandPollHandle commandAddr = (CommandPollHandle) command->implementer;

  return CommandPoll_getName(commandAddr);
}

/******************************************************************************
 Public functions
******************************************************************************/
CommandPollHandle CommandPoll_create(IScopeHandle scope){

  CommandPollHandle self = malloc(sizeof(CommandPollPrivateData));
  self->scope = scope;
  
  self->command.implementer = self;
  self->command.run = &run;
  self->command.setCommandAttribute = &setCommandAttribute;
  self->command.getCommandName = &getCommandName;

  self->nextTimeStamp = 0;

  return self;
}

ICommandHandle CommandPoll_getICommand(CommandPollHandle self){
  return &self->command;
}

char* CommandPoll_getName(CommandPollHandle self){
  return commandName;
}


void CommandPoll_destroy(CommandPollHandle self){
  free(self);
  self = NULL;
}
