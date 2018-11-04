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
static const char* commandName = "ev_poll";

/* Class data */
typedef struct __CommandPollPrivateData
{
  ICommand iCommand;
  IScopeHandle iScope;

  gemmi_uint nextTimeStamp;

} CommandPollPrivateData ;

/* Implementation of the run command, which will be passed into the interface */
static void run(ICommandHandle self);

/* Implementation of the getCommandName command, which will be passed into the interface */
static const char* getCommandName(ICommandHandle self);

/* Implementation of the setCommandAttribute command, which will be passed into the interface */
static void setCommandAttribute(ICommandHandle self, void* attr);

/******************************************************************************
 Private functions
******************************************************************************/
static void run(ICommandHandle self){
  CommandPollHandle commandPoll = (CommandPollHandle) self->implementer;
  commandPoll->iScope->poll(commandPoll->iScope, commandPoll->nextTimeStamp);
}

static void setCommandAttribute(ICommandHandle self, void* attr){
  CommandPollHandle commandPoll = (CommandPollHandle) self->implementer;

  commandPoll->nextTimeStamp = *(gemmi_uint*) attr;
}

static const char* getCommandName(ICommandHandle self){
  CommandPollHandle commandAddr = (CommandPollHandle) self->implementer;

  return CommandPoll_getName(commandAddr);
}

/******************************************************************************
 Public functions
******************************************************************************/
CommandPollHandle CommandPoll_create(IScopeHandle iScope){

  CommandPollHandle self = malloc(sizeof(CommandPollPrivateData));
  self->iScope = iScope;
  
  self->iCommand.implementer = self;
  self->iCommand.run = &run;
  self->iCommand.setCommandAttribute = &setCommandAttribute;
  self->iCommand.getCommandName = &getCommandName;

  self->nextTimeStamp = 0;

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
