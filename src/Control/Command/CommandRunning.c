/*!****************************************************************************************************************************************
 * @file         CommandRunning.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Control/Command/CommandRunning.h>

/******************************************************************************
 Define private data
******************************************************************************/

/* Class data */
typedef struct __CommandRunningPrivateData
{
  ICommand command;
  size_t amountOfChannels;
  CommandRunningConf config;
  IScopeHandle scope;

} CommandRunningPrivateData ;

/******************************************************************************
 Private functions
******************************************************************************/
static void run(ICommandHandle command){
  CommandRunningHandle self = (CommandRunningHandle) command->handle;

  for (size_t i = 0; i < self->config.numberOfChangedChannels; i++) {
    const uint32_t idOfChannelChanged = self->config.changedChannels[i];

    if(idOfChannelChanged >= self->amountOfChannels){
      return;
    }

    if(self->config.newStates[i] == CHANNEL_RUNNING){
      self->scope->setChannelRunning(self->scope, idOfChannelChanged);
    } else if(self->config.newStates[i] == CHANNEL_STOPPED) {
      self->scope->setChannelStopped(self->scope, idOfChannelChanged);
    }
  }
}

/******************************************************************************
 Public functions
******************************************************************************/
CommandRunningHandle CommandRunning_create(IScopeHandle scope){

  CommandRunningHandle self = malloc(sizeof(CommandRunningPrivateData));

  self->scope = scope;
  self->amountOfChannels = self->scope->getAmountOfChannels(self->scope);

  self->config.newStates = malloc(sizeof(CHANNEL_STATES) * self->amountOfChannels);
  self->config.changedChannels = malloc(sizeof(int) * self->amountOfChannels);
  self->config.numberOfChangedChannels = 0;

  self->command.handle = self;
  self->command.run = &run;

  return self;
}

ICommandHandle CommandRunning_getICommand(CommandRunningHandle self){
  return &self->command;
}

void CommandRunning_setAttributes(CommandRunningHandle self, CommandRunningConf conf){

  /* Safety checks */
  if(conf.numberOfChangedChannels > self->amountOfChannels){
    return;
  }

  for (size_t i = 0; i < conf.numberOfChangedChannels; i++) {
    if(conf.changedChannels[i] > self->amountOfChannels){
      return;
    }
    if(conf.newStates[i] != CHANNEL_STOPPED
      && conf.newStates[i] != CHANNEL_RUNNING){
      return;
    }
  }

  /* Copy data to command */
  self->config.numberOfChangedChannels = conf.numberOfChangedChannels;
  for (size_t i = 0; i < conf.numberOfChangedChannels; i++) {
    self->config.changedChannels[i] = conf.changedChannels[i];
    self->config.newStates[i] = conf.newStates[i];
  }
}

void CommandRunning_destroy(CommandRunningHandle self){
  free(self->config.newStates);
  self->config.newStates = NULL;
  free(self->config.changedChannels);
  self->config.changedChannels = NULL;
  free(self);
  self = NULL;
}
