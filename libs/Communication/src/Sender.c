/*!****************************************************************************************************************************************
 * @file         Sender.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Communication/Sender.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __SenderPrivateData
{
  IPackerHandle packer;
  COM_TYPE comType;
  TriggerHandle trigger;
  IScopeHandle scope;

  size_t numberOfChannels;
  ChannelHandle* channels;

} SenderPrivateData ;

/******************************************************************************
 Private functions
******************************************************************************/

/******************************************************************************
 Public functions
******************************************************************************/
/* Constructor: Creates a new instance of the Sender */
SenderHandle Sender_create(IPackerHandle packer, ChannelHandle* channels, const size_t numberOfChannels,
                           COM_TYPE comType,
                           TriggerHandle trigger,
                           IScopeHandle scope){

  SenderHandle self = (SenderHandle) malloc(sizeof(SenderPrivateData));

  self->comType = comType;
  self->packer = packer;
  self->trigger = trigger;
  self->scope = scope;

  self->channels = channels;
  self->numberOfChannels = numberOfChannels;

  return self;
}

/* Prepares a data package with channel and trigger data */
void Sender_pack(SenderHandle self){

  for (size_t i = 0; i < self->numberOfChannels; ++i) {
    if(Channel_isRunning(self->channels[i]) == true){
      self->packer->prepareChannel(self->packer, Channel_getRingBufferFloatStream(self->channels[i]), (const uint32_t) i);
    }
  }

  const bool isTriggered = Trigger_isTriggered(self->trigger);
  const uint32_t channelId = Trigger_getChannelId(self->trigger);
  const uint32_t triggerTimestamp = Trigger_getTriggerIndex(self->trigger);

  self->packer->prepareTrigger(self->packer, isTriggered, channelId, triggerTimestamp);

  const uint32_t scopeTimestamp = self->scope->getTimestamp(self->scope);
  self->packer->prepareTimestamp(self->packer, scopeTimestamp);

  const uint32_t timeIncrement = self->scope->getTimeIncrement(self->scope);
  self->packer->prepareTimeIncrement(self->packer, timeIncrement);

  self->packer->pack(self->packer);
}

/* Transmits a data package */
bool Sender_transmit(SenderHandle self);

/* Deconstructor: Deletes the instance of the Sender */
void Sender_destroy(SenderHandle self){
  free(self);
  self = NULL;
}