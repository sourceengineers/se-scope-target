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
// das gehört zu json -> und drum in den parser!
static char* commandName = "cf_addr";

/* Class data */
typedef struct __CommandAddrPrivateData
{
  ICommand command;
  
  CommandAddrConf config;
  size_t amountOfChannels;
  ChannelHandle* channels;
  
} CommandAddrPrivateData ;

/******************************************************************************
 Private functions
******************************************************************************/
static void run(ICommandHandle command){
  CommandAddrHandle self = (CommandAddrHandle) command->implementer;
  
  /* Set the polling address of the channels */
  for (size_t i = 0; i < self->config.numberOfChangedChannels; i++) {
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

static char* getCommandName(ICommandHandle command){
  CommandAddrHandle self = (CommandAddrHandle) command->implementer;

  return CommandAddr_getName(self);
}

static void setCommandAttribute(ICommandHandle command, void* attr){
  CommandAddrHandle self = (CommandAddrHandle) command->implementer;

  CommandAddrConf newConfig = *(CommandAddrConf*) attr;
  
  /* Safety checks */
  if(newConfig.numberOfChangedChannels > self->amountOfChannels){
    return;
  }
  
  /* Check that no id is bigger than the maximum ammount of channels */
  for (size_t i = 0; i < newConfig.numberOfChangedChannels; i++) {
    if(newConfig.changedChannels[i] > self->amountOfChannels){
      return;
    }
    if(newConfig.newAddresses[i] == NULL){
      return;
    }
  }
  
  /* Copy data to command */
  self->config.numberOfChangedChannels = newConfig.numberOfChangedChannels;
  for (size_t i = 0; i < newConfig.numberOfChangedChannels; i++) {
    self->config.changedChannels[i] = newConfig.changedChannels[i]; 
    self->config.newAddresses[i] = newConfig.newAddresses[i];
    self->config.types[i] = newConfig.types[i];
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
  
  self->command.implementer = self;
  self->command.run = &run;
  self->command.setCommandAttribute = &setCommandAttribute;
  self->command.getCommandName = &getCommandName;
  
  return self;
}

/******************************************************************************
 Public functions
******************************************************************************/
ICommandHandle CommandAddr_getICommand(CommandAddrHandle self){
  return &self->command;
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
