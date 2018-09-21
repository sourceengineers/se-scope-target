/*!****************************************************************************************************************************************
 * @file         CommandRunning.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Command/CommandRunning.h>
#include <Scope/Channel.h>

const char* commandName = "cf_running";

/* Define public data */
typedef struct __CommandRunningPrivateData
{
  ICommand iCommand;
  size_t ammountOfChannels;
  CommandRunningConfig config;
  ChannelHandle* channels;
  
} CommandRunningPrivateData ;

static void run(ICommandHandle self){
  CommandRunningHandle commandRunning = (CommandRunningHandle) self->implementer;
  
  for (size_t i = 0; i < ammountOfChannels; i++) {
    if(config.channelHasChanged[i] == true){
      if(config.setRunning[i] == CHANNEL_RUNNING){
        Channel_setStateRunning(commandRunning->channels[i]);
      }
      if(config.setRunning[i] == CHANNEL_STOPPED) {
        Channel_setStateStopped(commandRunning->channels[i]);
      }
    }
  }
}

static void setCommandAttribute(ICommandHandle self, void* attr){
  CommandRunningHandle commandRunning = (CommandRunningHandle) self->implementer;
  CommandRunningConfig conf = *(CommandRunningConfig*) attr;
  
  memcpy(commandRunning->conf.setRunning, conf.setRunning, self->ammountOfChannels);
  memcpy(commandRunning->conf.channelHasChanged, conf.channelHasChanged, self->ammountOfChannels);
}

/* Public functions */
CommandRunningHandle CommandRunning_create(ChannelHandle* channels, size_t ammountOfChannels){

  CommandRunningHandle self = malloc(sizeof(CommandRunningPrivateData));
  self->config.setRunning = malloc(sizeof(bool) * ammountOfChannels);
  self->config.channelHasChanged = (sizeof(bool) * ammountOfChannels);
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

char* CommandRunning_getName(CommandRunningHandle self){
  return commandName;
}


void CommandRunning_destroy(ScopeHandle self){
  free(self->conf.setRunning);
  free(self->conf.channelHasChanged);
  free(self);
}
