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
static char* commandName = "cf_tgr";

/* Class data */
typedef struct __CommandTriggerPrivateData
{
  ICommand command;
  
  TriggerHandle trigger;
  TriggerConfiguration config;
  
} CommandTriggerPrivateData ;

/******************************************************************************
 Private functions
******************************************************************************/
static void run(ICommandHandle command){
  CommandTriggerHandle self = (CommandTriggerHandle) command->implementer;
  
  Trigger_configure(self->trigger, self->config);
}

static void setCommandAttribute(ICommandHandle command, void* attr){
  CommandTriggerHandle self = (CommandTriggerHandle) command->implementer;
  
  TriggerConfiguration newConfig = *(TriggerConfiguration*) attr;
  self->config = newConfig;
}

static char* getCommandName(ICommandHandle command){
  CommandTriggerHandle self = (CommandTriggerHandle) command->implementer;

  return CommandTrigger_getName(self);
}

/******************************************************************************
Public functions
******************************************************************************/
CommandTriggerHandle CommandTrigger_create(TriggerHandle trigger){

  CommandTriggerHandle self = malloc(sizeof(CommandTriggerPrivateData));
  self->trigger = trigger;
  
  self->command.implementer = self;
  self->command.run = &run;
  self->command.setCommandAttribute = &setCommandAttribute;
  self->command.getCommandName = &getCommandName;

  return self;
}

ICommandHandle CommandTrigger_getICommand(CommandTriggerHandle self){
  return &self->command;
}

char* CommandTrigger_getName(CommandTriggerHandle self){
  return commandName;
}

void CommandTrigger_destroy(CommandTriggerHandle self){
  free(self);
  self = NULL;
}
