/*!****************************************************************************************************************************************
 * @file         CommandTrans.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Command/CommandTrans.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Name of the command */
static char* commandName = "ev_trans";

/* Class data */
typedef struct __CommandTransPrivateData
{
  ICommand command;
  IScopeHandle scope;
  
} CommandTransPrivateData ;

/******************************************************************************
 Private functions
******************************************************************************/
static void run(ICommandHandle command){
  CommandTransHandle self = (CommandTransHandle) command->implementer;
  self->scope->transmit(self->scope);
}

static void setCommandAttribute(ICommandHandle command, void* attr){
  return;
}

static char* getCommandName(ICommandHandle command){
  CommandTransHandle self = (CommandTransHandle) command->implementer;

  return CommandTrans_getName(self);
}

/******************************************************************************
 Private functions
******************************************************************************/
CommandTransHandle CommandTrans_create(IScopeHandle scope){

  CommandTransHandle self = malloc(sizeof(CommandTransPrivateData));
  self->scope = scope;
  
  self->command.implementer = self;
  self->command.run = &run;
  self->command.setCommandAttribute = &setCommandAttribute;
  self->command.getCommandName = &getCommandName;

  return self;
}

ICommandHandle CommandTrans_getICommand(CommandTransHandle self){
  return &self->command;
}

char* CommandTrans_getName(CommandTransHandle self){
  return commandName;
}


void CommandTrans_destroy(CommandTransHandle self){
  free(self);
  self = NULL;
}
