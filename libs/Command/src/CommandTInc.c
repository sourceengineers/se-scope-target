/*!****************************************************************************************************************************************
 * @file         CommandTInc.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Command/CommandTInc.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Name of the command */
static const char* commandName = "cf_t_inc";

/* Class data */
typedef struct __CommandTIncPrivateData
{
  ICommand iCommand;
  IScopeHandle iScope;
  
  int timeIncrement;
} CommandTIncPrivateData ;

/******************************************************************************
 Private functions
******************************************************************************/
static void run(ICommandHandle self){
  CommandTIncHandle commandTInc = (CommandTIncHandle) self->implementer;
  commandTInc->iScope->setTimeIncrement(commandTInc->iScope, commandTInc->timeIncrement);
}

static void setCommandAttribute(ICommandHandle self, void* attr){
  CommandTIncHandle commandTInc = (CommandTIncHandle) self->implementer;
  commandTInc->timeIncrement = *(int*) attr;
}

static const char* getCommandName(ICommandHandle self){
  CommandTIncHandle commandAddr = (CommandTIncHandle) self->implementer;

  return CommandTInc_getName(commandAddr);
}

/******************************************************************************
 Private functions
******************************************************************************/
CommandTIncHandle CommandTInc_create(IScopeHandle iScope){

  CommandTIncHandle self = malloc(sizeof(CommandTIncPrivateData));
  self->iScope = iScope;
  self->timeIncrement = 0;
  
  self->iCommand.implementer = self;
  self->iCommand.run = &run;
  self->iCommand.setCommandAttribute = &setCommandAttribute;
  self->iCommand.getCommandName = &getCommandName;

  return self;
}

ICommandHandle CommandTInc_getICommand(CommandTIncHandle self){
  return &self->iCommand;
}

const char* CommandTInc_getName(CommandTIncHandle self){
  return commandName;
}


void CommandTInc_destroy(CommandTIncHandle self){
  free(self);
  self = NULL;
}
