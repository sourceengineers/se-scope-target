/*!****************************************************************************************************************************************
 * @file         CommandRunning.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Command/CommandRunning.h>

const char* commandName = "cf_running";

/* Define public data */
typedef struct __CommandRunningPrivateData
{
  ICommand iCommand;
  size_t ammountOfChannels;
  CHANNEL_STATES* config;
  ChannelHandle* channels;
  
} CommandRunningPrivateData ;

static void run(ICommandHandle self){
  CommandRunningHandle commandRunning = (CommandRunningHandle) self->implementer;
  
  for (size_t i = 0; i < commandRunning->ammountOfChannels; i++) {
    if(commandRunning->config[i] == CHANNEL_RUNNING){
      Channel_setStateRunning(commandRunning->channels[i]);
    } else if(commandRunning->config[i] == CHANNEL_STOPPED) {
      Channel_setStateStopped(commandRunning->channels[i]);
    }
  }
}

static void setCommandAttribute(ICommandHandle self, void* attr){
  CommandRunningHandle commandRunning = (CommandRunningHandle) self->implementer;
  CHANNEL_STATES* conf = (CHANNEL_STATES*) attr;
  
  for (size_t i = 0; i < commandRunning->ammountOfChannels; i++) {
    commandRunning->config[i] = conf[i];
  }
}

/* Public functions */
CommandRunningHandle CommandRunning_create(ChannelHandle* channels, size_t ammountOfChannels){

  CommandRunningHandle self = malloc(sizeof(CommandRunningPrivateData));
  self->config = malloc(sizeof(CHANNEL_STATES) * ammountOfChannels);
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
  free(self->config);
  free(self);
}
