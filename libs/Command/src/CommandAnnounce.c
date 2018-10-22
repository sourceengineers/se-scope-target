/*!****************************************************************************************************************************************
 * @file         CommandAnnounce.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Command/CommandAnnounce.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Name of the command */
static const char* commandName = "ev_announce";

/* Class data */
typedef struct __CommandAnnouncePrivateData
{
  ICommand iCommand;
  IScopeHandle iScope;

} CommandAnnouncePrivateData ;

/******************************************************************************
 Private functions
******************************************************************************/
static void run(ICommandHandle self){
  CommandAnnounceHandle commandAnnounce = (CommandAnnounceHandle) self->implementer;
  commandAnnounce->iScope->announce(commandAnnounce->iScope);
}

static void setCommandAttribute(ICommandHandle self, void* attr){
  return;
}

static const char* getCommandName(ICommandHandle self){
  CommandAnnounceHandle commandAddr = (CommandAnnounceHandle) self->implementer;

  return CommandAnnounce_getName(commandAddr);
}

/******************************************************************************
 Private functions
******************************************************************************/
CommandAnnounceHandle CommandAnnounce_create(IScopeHandle iScope){

  CommandAnnounceHandle self = malloc(sizeof(CommandAnnouncePrivateData));
  self->iScope = iScope;
  
  self->iCommand.implementer = self;
  self->iCommand.run = &run;
  self->iCommand.setCommandAttribute = &setCommandAttribute;
  self->iCommand.getCommandName = &getCommandName;

  return self;
}

ICommandHandle CommandAnnounce_getICommand(CommandAnnounceHandle self){
  return &self->iCommand;
}

const char* CommandAnnounce_getName(CommandAnnounceHandle self){
  return commandName;
}


void CommandAnnounce_destroy(CommandAnnounceHandle self){
  free(self);
  self = NULL;
}
