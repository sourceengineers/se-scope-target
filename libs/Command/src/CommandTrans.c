/*!****************************************************************************************************************************************
 * @file         CommandTrans.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Command/CommandTrans.h>

static const char* commandName = "ev_trans";

/* Define public data */
typedef struct __CommandTransPrivateData
{
  ICommand iCommand;
  IScopeHandle iScope;
  
} CommandTransPrivateData ;

static void run(ICommandHandle self){
  CommandTransHandle commandTrans = (CommandTransHandle) self->implementer;
  commandTrans->iScope->trans(commandTrans->iScope);
}

static void setCommandAttribute(ICommandHandle self, void* attr){
  return;
}

static const char* getCommandName(ICommandHandle self){
  CommandTransHandle commandAddr = (CommandTransHandle) self->implementer;

  return CommandTrans_getName(commandAddr);
}

/* Public functions */
CommandTransHandle CommandTrans_create(IScopeHandle iScope){

  CommandTransHandle self = malloc(sizeof(CommandTransPrivateData));
  self->iScope = iScope;
  
  self->iCommand.implementer = self;
  self->iCommand.run = &run;
  self->iCommand.setCommandAttribute = &setCommandAttribute;
  self->iCommand.getCommandName = &getCommandName;

  return self;
}

ICommandHandle CommandTrans_getICommand(CommandTransHandle self){
  return &self->iCommand;
}

const char* CommandTrans_getName(CommandTransHandle self){
  return commandName;
}


void CommandTrans_destroy(CommandTransHandle self){
  free(self);
  self = NULL;
}
