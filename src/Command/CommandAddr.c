/*!****************************************************************************************************************************************
 * @file         CommandAddr.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Command/CommandAddr.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandAddrPrivateData{
    ICommand command;

    CommandAddrConf config;
    size_t amountOfChannels;
    ChannelHandle* channels;

} CommandAddrPrivateData;

/******************************************************************************
 Private functions
******************************************************************************/
static void run(ICommandHandle command){
  CommandAddrHandle self = (CommandAddrHandle) command->handle;

  /* Set the polling address of the channels */
  for(size_t i = 0; i < self->config.numberOfChangedChannels; i++){
    const int idChangingChannel = self->config.changedChannels[i];

    if(idChangingChannel >= self->amountOfChannels){
      return;
    }

    ChannelHandle changingChannel = self->channels[idChangingChannel];
    const DATA_TYPES newType = self->config.types[i];
    void* newAddress = self->config.newAddresses[i];

    Channel_setPollAddress(changingChannel, newAddress, newType);
  }
}

/******************************************************************************
 Public functions
******************************************************************************/
CommandAddrHandle CommandAddr_create(ChannelHandle* channels, size_t amountOfChannels){

  CommandAddrHandle self = malloc(sizeof(CommandAddrPrivateData));
  self->config.newAddresses = malloc(sizeof(void*) * amountOfChannels);
  self->config.changedChannels = malloc(sizeof(int) * amountOfChannels);
  self->config.types = malloc(sizeof(DATA_TYPES) * amountOfChannels);

  self->config.numberOfChangedChannels = 0;

  self->channels = channels;
  self->amountOfChannels = amountOfChannels;

  self->command.handle = self;
  self->command.run = &run;

  return self;
}

ICommandHandle CommandAddr_getICommand(CommandAddrHandle self){
  return &self->command;
}

void CommandAddr_setAttributes(CommandAddrHandle self, CommandAddrConf conf){

  /* Safety checks */
  if(conf.numberOfChangedChannels > self->amountOfChannels){
    return;
  }

  /* Check that no id is bigger than the maximum ammount of channels */
  for(size_t i = 0; i < conf.numberOfChangedChannels; i++){
    if(conf.changedChannels[i] > self->amountOfChannels){
      return;
    }
    if(conf.newAddresses[i] == NULL){
      return;
    }
  }

  /* Copy data to command */
  self->config.numberOfChangedChannels = conf.numberOfChangedChannels;
  for(size_t i = 0; i < conf.numberOfChangedChannels; i++){
    self->config.changedChannels[i] = conf.changedChannels[i];
    self->config.newAddresses[i] = conf.newAddresses[i];
    self->config.types[i] = conf.types[i];
  }
}

void CommandAddr_destroy(CommandAddrHandle self){
  free(self->config.newAddresses);
  self->config.newAddresses = NULL;
  free(self->config.changedChannels);
  self->config.changedChannels = NULL;
  free(self->config.types);
  self->config.types = NULL;
  free(self);
  self = NULL;
}
