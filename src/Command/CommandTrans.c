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
/* Class data */
typedef struct __CommandTransPrivateData{
    ICommand command;
    IScopeHandle scope;

} CommandTransPrivateData;

/******************************************************************************
 Private functions
******************************************************************************/
static void run(ICommandHandle command){
  CommandTransHandle self = (CommandTransHandle) command->handle;
  self->scope->transmit(self->scope);
}

static void setCommandAttribute(ICommandHandle command, void* attr){
  return;
}

/******************************************************************************
 Private functions
******************************************************************************/
CommandTransHandle CommandTrans_create(IScopeHandle scope){

  CommandTransHandle self = malloc(sizeof(CommandTransPrivateData));
  self->scope = scope;

  self->command.handle = self;
  self->command.run = &run;

  return self;
}

ICommandHandle CommandTrans_getICommand(CommandTransHandle self){
  return &self->command;
}

void CommandTrans_destroy(CommandTransHandle self){
  free(self);
  self = NULL;
}
