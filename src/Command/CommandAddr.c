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
/* Name of the command */
static char* commandName = "cf_addr";

/* Class data */
typedef struct __CommandAddrPrivateData
{
  ICommand iCommand;
  
  CommandAddrConf config;
  
  size_t amountOfChannels;
  ChannelHandle* channels;
  
} CommandAddrPrivateData ;

/******************************************************************************
 Private functions
******************************************************************************/
static void run(ICommandHandle self){
  CommandAddrHandle commandAddr = (CommandAddrHandle) self->implementer;
  
  /* Set the polling address of the channels */
  for (size_t i = 0; i < commandAddr->config.numberOfChangedChannels; i++) {
    const int idChangingChannel = commandAddr->config.changedChannels[i];

    if(idChangingChannel >= commandAddr->amountOfChannels){
      return;
    }

    ChannelHandle changingChannel = commandAddr->channels[idChangingChannel];
    const DATA_TYPES newType = commandAddr->config.types[i];
    void* newAddress = commandAddr->config.newAddresses[i];
    
    Channel_setPollAddress(changingChannel, newAddress, newType);
  }
}

static char* getCommandName(ICommandHandle self){
  CommandAddrHandle commandAddr = (CommandAddrHandle) self->implementer;

  return CommandAddr_getName(commandAddr);
}

static void setCommandAttribute(ICommandHandle self, void* attr){
  CommandAddrHandle commandAddr = (CommandAddrHandle) self->implementer;

  CommandAddrConf newConfig = *(CommandAddrConf*) attr;
  
  /* Safety checks */
  if(newConfig.numberOfChangedChannels > commandAddr->amountOfChannels){
    return;
  }
  
  /* Check that no id is bigger than the maximum ammount of channels */
  for (size_t i = 0; i < newConfig.numberOfChangedChannels; i++) {
    if(newConfig.changedChannels[i] > commandAddr->amountOfChannels){
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
CommandAddrHandle CommandAddr_create(ChannelHandle* channels, size_t amountOfChannels){

  CommandAddrHandle self = malloc(sizeof(CommandAddrPrivateData));
  self->config.newAddresses = malloc(sizeof(void*) * amountOfChannels);
  self->config.changedChannels = malloc(sizeof(int) * amountOfChannels);
  self->config.types = malloc(sizeof(DATA_TYPES) * amountOfChannels);
  
  self->config.numberOfChangedChannels = 0;

  self->channels = channels;
  self->amountOfChannels = amountOfChannels;
  
  self->iCommand.implementer = self;
  self->iCommand.run = &run;
  self->iCommand.setCommandAttribute = &setCommandAttribute;
  self->iCommand.getCommandName = &getCommandName;
  
  return self;
}

/******************************************************************************
 Public functions
******************************************************************************/
ICommandHandle CommandAddr_getICommand(CommandAddrHandle self){
  return &self->iCommand;
}

char* CommandAddr_getName(CommandAddrHandle self){
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
