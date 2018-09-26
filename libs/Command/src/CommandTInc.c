/*!****************************************************************************************************************************************
 * @file         CommandTInc.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Command/CommandTInc.h>

const char* commandName = "ev_t_inc";

/* Define public data */
typedef struct __CommandTIncPrivateData
{
  ICommand iCommand;
  IScopeHandle iScope;
  
  int timeIncrement;
} CommandTIncPrivateData ;

static void run(ICommandHandle self){
  CommandTIncHandle commandTInc = (CommandTIncHandle) self->implementer;
  commandTInc->iScope->setTimeIncrement(commandTInc->iScope, commandTInc->timeIncrement);
}

static void setCommandAttribute(ICommandHandle self, void* attr){
  CommandTIncHandle commandTInc = (CommandTIncHandle) self->implementer;
  commandTInc->timeIncrement = *(int*) attr;
}

/* Public functions */
CommandTIncHandle CommandTInc_create(IScopeHandle iScope){

  CommandTIncHandle self = malloc(sizeof(CommandTIncPrivateData));
  self->iScope = iScope;
  self->timeIncrement = 0;
  
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
}
