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
  ICommand command;
  IScopeHandle scope;

} CommandClearPrivateData ;

/******************************************************************************
 Private functions
******************************************************************************/
static void run(ICommandHandle command){
  CommandClearHandle self = (CommandClearHandle) command->implementer;
  self->scope->clear(self->scope);
}

static void setCommandAttribute(ICommandHandle command, void* attr){
  return;
}

static char* getCommandName(ICommandHandle command){
  CommandClearHandle commandAddr = (CommandClearHandle) command->implementer;

  return CommandClear_getName(commandAddr);
}

/******************************************************************************
 Private functions
******************************************************************************/
CommandClearHandle CommandClear_create(IScopeHandle scope){

  CommandClearHandle self = malloc(sizeof(CommandClearPrivateData));
  self->scope = scope;
  
  self->command.implementer = self;
  self->command.run = &run;
  self->command.setCommandAttribute = &setCommandAttribute;
  self->command.getCommandName = &getCommandName;

  return self;
}

ICommandHandle CommandClear_getICommand(CommandClearHandle self){
  return &self->command;
}

char* CommandClear_getName(CommandClearHandle self){
  return commandName;
}


void CommandClear_destroy(CommandClearHandle self){
  free(self);
  self = NULL;
}
