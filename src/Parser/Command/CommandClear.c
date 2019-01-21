/*!****************************************************************************************************************************************
 * @file         CommandClear.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Parser/Command/CommandClear.h>

/******************************************************************************
 Define private data
******************************************************************************/
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
  CommandClearHandle self = (CommandClearHandle) command->handle;
  self->scope->clear(self->scope);
}

/******************************************************************************
 Private functions
******************************************************************************/
CommandClearHandle CommandClear_create(IScopeHandle scope){

  CommandClearHandle self = malloc(sizeof(CommandClearPrivateData));
  self->scope = scope;
  
  self->command.handle = self;
  self->command.run = &run;

  return self;
}

ICommandHandle CommandClear_getICommand(CommandClearHandle self){
  return &self->command;
}

void CommandClear_destroy(CommandClearHandle self){
  free(self);
  self = NULL;
}
