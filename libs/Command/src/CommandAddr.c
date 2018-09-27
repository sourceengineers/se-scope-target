/*!****************************************************************************************************************************************
 * @file         CommandAddr.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Command/CommandAddr.h>

static const char* commandName = "cf_addr";

/* Define public data */
typedef struct __CommandAddrPrivateData
{
  ICommand iCommand;
  
  CommandAddrConf config;
  
  size_t ammountOfChannels;
  ChannelHandle* channels;
  
} CommandAddrPrivateData ;

static void run(ICommandHandle self){
  CommandAddrHandle commandAddr = (CommandAddrHandle) self->implementer;
  
  /* Set the polling address of the channels */
  for (size_t i = 0; i < commandAddr->config.numberOfChangedChannels; i++) {
    const int idChangingChannel = commandAddr->config.changedChannels[i];
    ChannelHandle changingChannel = commandAddr->channels[idChangingChannel];
    const DATA_TYPES newType = commandAddr->config.types[i];
    void* newAddress = commandAddr->config.newAddresses[i];
    
    Channel_setPollAddress(changingChannel, newAddress, newType);
  }
}

static void setCommandAttribute(ICommandHandle self, void* attr){
  CommandAddrHandle commandAddr = (CommandAddrHandle) self->implementer;

  CommandAddrConf newConfig = *(CommandAddrConf*) attr;
  
  /* Safety checks */
  if(newConfig.numberOfChangedChannels > commandAddr->ammountOfChannels){
    return;
  }
  
  for (size_t i = 0; i < newConfig.numberOfChangedChannels; i++) {
    if(newConfig.changedChannels[i] > commandAddr->ammountOfChannels){
      return;
    }
    if(newConfig.newAddresses[i] == NULL){
      return;
    }
  }
  
  /* Copy data to command */
  commandAddr->config.numberOfChangedChannels = newConfig.numberOfChangedChannels;
  for (size_t i = 0; i < newConfig.numberOfChangedChannels; i++) {
    commandAddr->config.changedChannels[i] = newConfig.changedChannels[i]; 
    commandAddr->config.newAddresses[i] = newConfig.newAddresses[i];
    commandAddr->config.types[i] = newConfig.types[i];
  }
}

/* Public functions */
CommandAddrHandle CommandAddr_create(ChannelHandle* channels, size_t ammountOfChannels){

  CommandAddrHandle self = malloc(sizeof(CommandAddrPrivateData));
  self->config.newAddresses = malloc(sizeof(void*) * ammountOfChannels);
  self->config.changedChannels = malloc(sizeof(int) * ammountOfChannels);
  self->config.types = malloc(sizeof(DATA_TYPES) * ammountOfChannels);
  
  self->config.numberOfChangedChannels = 0;

  self->channels = channels;
  self->ammountOfChannels = ammountOfChannels;
  
  self->iCommand.implementer = self;
  self->iCommand.run = &run;
  self->iCommand.setCommandAttribute = &setCommandAttribute;
  
  return self;
}

ICommandHandle CommandAddr_getICommand(CommandAddrHandle self){
  return &self->iCommand;
}

const char* CommandAddr_getName(CommandAddrHandle self){
  return commandName;
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
