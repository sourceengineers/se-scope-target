/*!****************************************************************************************************************************************
 * @file         CommandFactory.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Command/CommandFactory.h>
#include <string.h>


/* Define public data */
typedef struct __CommandFactoryPrivateData
{
  CommandRunningHandle commandRunning;
  CommandPollHandle commandPoll;
  CommandAddrHandle commandAddr;
  CommandTIncHandle commandTInc;
  CommandTransHandle commandTrans;
  CommandTriggerHandle commandTrigger;
  
} CommandFactoryPrivateData ;

/* Public functions */
CommandFactoryHandle CommandFactory_create(IScopeHandle iScope, 
                                           ChannelHandle* channels, 
                                           size_t ammountOfChannels,
                                           TriggerHandle trigger){

  CommandFactoryHandle self = malloc(sizeof(CommandFactoryPrivateData));
  self->commandRunning = CommandRunning_create(channels, ammountOfChannels);
  self->commandPoll = CommandPoll_create(iScope);
  self->commandAddr = CommandAddr_create(channels, ammountOfChannels);
  self->commandTInc = CommandTInc_create(iScope);
  self->commandTrans = CommandTrans_create(iScope);
  self->commandTrigger = CommandTrigger_create(trigger);

  return self;
}

ICommandHandle CommandFactory_getICommand(CommandFactoryHandle self, char* command){

  if(strcpy(command, CommandRunning_getName(self->commandRunning)) == 0){
    return CommandRunning_getICommand(self->commandRunning);
  } else if(strcpy(command, CommandPoll_getName(self->commandPoll)) == 0){
    return CommandPoll_getICommand(self->commandPoll);
  } else if(strcpy(command, CommandAddr_getName(self->commandAddr)) == 0){
    return CommandAddr_getICommand(self->commandAddr);
  } else if(strcpy(command, CommandTInc_getName(self->commandTInc)) == 0){
    return CommandTInc_getICommand(self->commandTInc);
  } else if(strcpy(command, CommandTrans_getName(self->commandTrans)) == 0){
    return CommandTrans_getICommand(self->commandTrans);
  } else if(strcpy(command, CommandTrigger_getName(self->commandTrigger)) == 0){
    return CommandTrigger_getICommand(self->commandTrigger);
  }
  
  return NULL;
}

void CommandFactory_destroy(CommandFactoryHandle self){
  free(self);
}
