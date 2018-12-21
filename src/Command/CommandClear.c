/*!****************************************************************************************************************************************
 * @file         CommandClear.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Command/CommandClear.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Name of the command */
static char* commandName = "ev_clear";

/* Class data */
typedef struct __CommandClearPrivateData
{
  ICommand iCommand;
  IScopeHandle iScope;

} CommandClearPrivateData ;

/******************************************************************************
 Private functions
******************************************************************************/
static void run(ICommandHandle self){
  CommandClearHandle commandClear = (CommandClearHandle) self->implementer;
  commandClear->iScope->clear(commandClear->iScope);
}

static void setCommandAttribute(ICommandHandle self, void* attr){
  return;
}

static char* getCommandName(ICommandHandle self){
  CommandClearHandle commandAddr = (CommandClearHandle) self->implementer;

  return CommandClear_getName(commandAddr);
}

/******************************************************************************
 Private functions
******************************************************************************/
CommandClearHandle CommandClear_create(IScopeHandle iScope){

  CommandClearHandle self = malloc(sizeof(CommandClearPrivateData));
  self->iScope = iScope;
  
  self->iCommand.implementer = self;
  self->iCommand.run = &run;
  self->iCommand.setCommandAttribute = &setCommandAttribute;
  self->iCommand.getCommandName = &getCommandName;

  return self;
}

ICommandHandle CommandClear_getICommand(CommandClearHandle self){
  return &self->iCommand;
}

char* CommandClear_getName(CommandClearHandle self){
  return commandName;
}


void CommandClear_destroy(CommandClearHandle self){
  free(self);
  self = NULL;
}
