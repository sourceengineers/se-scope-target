/*!****************************************************************************************************************************************
 * @file         CommandRunning.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Command/CommandRunning.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Name of the command */
static char* commandName = "cf_running";

/* Class data */
typedef struct __CommandRunningPrivateData
{
  ICommand command;
  size_t amountOfChannels;
  CommandRunningConf config;
  ChannelHandle* channels;
  
} CommandRunningPrivateData ;

/******************************************************************************
 Private functions
******************************************************************************/
static void run(ICommandHandle command){
  CommandRunningHandle self = (CommandRunningHandle) command->implementer;
  
  for (size_t i = 0; i < self->config.numberOfChangedChannels; i++) {
    const int idOfChannelChanged = self->config.changedChannels[i];

    if(idOfChannelChanged >= self->amountOfChannels){
      return;
    }

    ChannelHandle changingChannel = self->channels[idOfChannelChanged];
    
    if(self->config.newStates[i] == CHANNEL_RUNNING){
      Channel_setStateRunning(changingChannel);
    } else if(self->config.newStates[i] == CHANNEL_STOPPED) {
      Channel_setStateStopped(changingChannel);
    }
  }
}

static void setCommandAttribute(ICommandHandle command, void* attr){
  CommandRunningHandle self = (CommandRunningHandle) command->implementer;

  CommandRunningConf newConfig = *(CommandRunningConf*) attr;
  
  /* Safety checks */
  if(newConfig.numberOfChangedChannels > self->amountOfChannels){
    return;
  }
  
  for (size_t i = 0; i < newConfig.numberOfChangedChannels; i++) {
    if(newConfig.changedChannels[i] > self->amountOfChannels){
      return;
    }
    if(newConfig.newStates[i] != CHANNEL_STOPPED 
      && newConfig.newStates[i] != CHANNEL_RUNNING){
      return;
    }
  }
  
  /* Copy data to command */
  self->config.numberOfChangedChannels = newConfig.numberOfChangedChannels;
  for (size_t i = 0; i < newConfig.numberOfChangedChannels; i++) {
    self->config.changedChannels[i] = newConfig.changedChannels[i]; 
    self->config.newStates[i] = newConfig.newStates[i];
  }
}


static char* getCommandName(ICommandHandle command){
  CommandRunningHandle commandAddr = (CommandRunningHandle) command->implementer;

  return CommandRunning_getName(commandAddr);
}

/******************************************************************************
 Public functions
******************************************************************************/
CommandRunningHandle CommandRunning_create(ChannelHandle* channels, const size_t amountOfChannels){

  CommandRunningHandle self = malloc(sizeof(CommandRunningPrivateData));
  self->config.newStates = malloc(sizeof(CHANNEL_STATES) * amountOfChannels);
  self->config.changedChannels = malloc(sizeof(int) * amountOfChannels);
  self->config.numberOfChangedChannels = 0;
  self->channels = channels;
  self->amountOfChannels = amountOfChannels;
  
  self->command.implementer = self;
  self->command.run = &run;
  self->command.setCommandAttribute = &setCommandAttribute;
  self->command.getCommandName = &getCommandName;

  return self;
}

ICommandHandle CommandRunning_getICommand(CommandRunningHandle self){
  return &self->command;
}

char* CommandRunning_getName(CommandRunningHandle self){
  return commandName;
}

void CommandRunning_destroy(CommandRunningHandle self){
  free(self->config.newStates);
  self->config.newStates = NULL;
  free(self->config.changedChannels);
  self->config.changedChannels = NULL;
  free(self);
  self = NULL;
}
