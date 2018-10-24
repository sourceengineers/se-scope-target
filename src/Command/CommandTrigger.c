/*!****************************************************************************************************************************************
 * @file         CommandTrigger.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Command/CommandTrigger.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Name of the command */
static const char* commandName = "cf_tgr";

/* Class data */
typedef struct __CommandTriggerPrivateData
{
  ICommand iCommand;
  
  TriggerHandle trigger;
  TriggerConfiguration config;
  
} CommandTriggerPrivateData ;

/******************************************************************************
 Private functions
******************************************************************************/
static void run(ICommandHandle self){
  CommandTriggerHandle commandTrigger = (CommandTriggerHandle) self->implementer;
  
  Trigger_configure(commandTrigger->trigger, commandTrigger->config);
}

static void setCommandAttribute(ICommandHandle self, void* attr){
  CommandTriggerHandle commandTrigger = (CommandTriggerHandle) self->implementer;
  
  TriggerConfiguration newConfig = *(TriggerConfiguration*) attr;
  commandTrigger->config = newConfig;
}

static const char* getCommandName(ICommandHandle self){
  CommandTriggerHandle commandAddr = (CommandTriggerHandle) self->implementer;

  return CommandTrigger_getName(commandAddr);
}

/******************************************************************************
Public functions
******************************************************************************/
CommandTriggerHandle CommandTrigger_create(TriggerHandle trigger){

  CommandTriggerHandle self = malloc(sizeof(CommandTriggerPrivateData));
  self->trigger = trigger;
  
  self->iCommand.implementer = self;
  self->iCommand.run = &run;
  self->iCommand.setCommandAttribute = &setCommandAttribute;
  self->iCommand.getCommandName = &getCommandName;

  return self;
}

ICommandHandle CommandTrigger_getICommand(CommandTriggerHandle self){
  return &self->iCommand;
}

const char* CommandTrigger_getName(CommandTriggerHandle self){
  return commandName;
}

void CommandTrigger_destroy(CommandTriggerHandle self){
  free(self);
  self = NULL;
}
