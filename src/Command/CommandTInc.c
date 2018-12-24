/*!****************************************************************************************************************************************
 * @file         CommandTInc.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Command/CommandTInc.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Name of the command */
static char* commandName = "cf_t_inc";

/* Class data */
typedef struct __CommandTIncPrivateData
{
  ICommand command;
  IScopeHandle scope;
  
  int timeIncrement;
} CommandTIncPrivateData ;

/******************************************************************************
 Private functions
******************************************************************************/
static void run(ICommandHandle command){
  CommandTIncHandle self = (CommandTIncHandle) command->implementer;
  self->scope->setTimeIncrement(self->scope, self->timeIncrement);
}

static void setCommandAttribute(ICommandHandle command, void* attr){
  CommandTIncHandle self = (CommandTIncHandle) command->implementer;
  self->timeIncrement = *(int*) attr;
}

static char* getCommandName(ICommandHandle command){
  CommandTIncHandle self = (CommandTIncHandle) command->implementer;

  return CommandTInc_getName(self);
}

/******************************************************************************
 Private functions
******************************************************************************/
CommandTIncHandle CommandTInc_create(IScopeHandle scope){

  CommandTIncHandle self = malloc(sizeof(CommandTIncPrivateData));
  self->scope = scope;
  self->timeIncrement = 0;
  
  self->command.implementer = self;
  self->command.run = &run;
  self->command.setCommandAttribute = &setCommandAttribute;
  self->command.getCommandName = &getCommandName;

  return self;
}

ICommandHandle CommandTInc_getICommand(CommandTIncHandle self){
  return &self->command;
}

char* CommandTInc_getName(CommandTIncHandle self){
  return commandName;
}


void CommandTInc_destroy(CommandTIncHandle self){
  free(self);
  self = NULL;
}
