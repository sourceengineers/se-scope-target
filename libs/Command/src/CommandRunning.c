/*!****************************************************************************************************************************************
 * @file         CommandRunning.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Command/CommandRunning.h>

static const char* commandName = "cf_running";

/* Define public data */
typedef struct __CommandRunningPrivateData
{
  ICommand iCommand;
  size_t ammountOfChannels;
  CommandRunningConf config;
  ChannelHandle* channels;
  
} CommandRunningPrivateData ;

static void run(ICommandHandle self){
  CommandRunningHandle commandRunning = (CommandRunningHandle) self->implementer;
  
  for (size_t i = 0; i < commandRunning->config.numberOfChangedChannels; i++) {
    const int idOfChannelChanged = commandRunning->config.changedChannels[i];
    ChannelHandle changingChannel = commandRunning->channels[idOfChannelChanged];
    
    if(commandRunning->config.newStates[i] == CHANNEL_RUNNING){
      Channel_setStateRunning(changingChannel);
    } else if(commandRunning->config.newStates[i] == CHANNEL_STOPPED) {
      Channel_setStateStopped(changingChannel);
    }
  }
}

static void setCommandAttribute(ICommandHandle self, void* attr){
  CommandRunningHandle commandRunning = (CommandRunningHandle) self->implementer;

  CommandRunningConf newConfig = *(CommandRunningConf*) attr;
  
  /* Safety checks */
  if(newConfig.numberOfChangedChannels > commandRunning->ammountOfChannels){
    return;
  }
  
  for (size_t i = 0; i < newConfig.numberOfChangedChannels; i++) {
    if(newConfig.changedChannels[i] > commandRunning->ammountOfChannels){
      return;
    }
    if(newConfig.newStates[i] != CHANNEL_STOPPED 
      && newConfig.newStates[i] != CHANNEL_RUNNING){
      return;
    }
  }
  
  /* Copy data to command */
  commandRunning->config.numberOfChangedChannels = newConfig.numberOfChangedChannels;
  for (size_t i = 0; i < newConfig.numberOfChangedChannels; i++) {
    commandRunning->config.changedChannels[i] = newConfig.changedChannels[i]; 
    commandRunning->config.newStates[i] = newConfig.newStates[i];
  }
}

/* Public functions */
CommandRunningHandle CommandRunning_create(ChannelHandle* channels, const size_t ammountOfChannels){

  CommandRunningHandle self = malloc(sizeof(CommandRunningPrivateData));
  self->config.newStates = malloc(sizeof(CHANNEL_STATES) * ammountOfChannels);
  self->config.changedChannels = malloc(sizeof(int) * ammountOfChannels);
  self->config.numberOfChangedChannels = 0;
  self->channels = channels;
  self->ammountOfChannels = ammountOfChannels;
  
  self->iCommand.implementer = self;
  self->iCommand.run = &run;
  self->iCommand.setCommandAttribute = &setCommandAttribute;
  
  return self;
}

ICommandHandle CommandRunning_getICommand(CommandRunningHandle self){
  return &self->iCommand;
}

const char* CommandRunning_getName(CommandRunningHandle self){
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
