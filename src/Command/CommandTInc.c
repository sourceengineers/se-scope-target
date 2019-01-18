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

/* Class data */
typedef struct __CommandTIncPrivateData{
    ICommand command;
    IScopeHandle scope;

    int timeIncrement;
} CommandTIncPrivateData;

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

  return self;
}

ICommandHandle CommandTInc_getICommand(CommandTIncHandle self){
  return &self->command;
}

void CommandTInc_destroy(CommandTIncHandle self){
  free(self);
  self = NULL;
}
