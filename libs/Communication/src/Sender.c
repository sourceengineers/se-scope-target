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
SenderHandle Sender_create(IPackerHandle packer, ChannelHandle* channels, const size_t numberOfChannels,
                           COM_TYPE comType,
                           TriggerHandle trigger,
                           IScopeHandle scope){

  SenderHandle self = (SenderHandle) malloc(sizeof(SenderPrivateData));

  self->packer = packer;
  self->trigger = trigger;
  self->scope = scope;

  self->channels = channels;
  self->numberOfChannels = numberOfChannels;

  return self;
}

void Sender_flowControl(SenderHandle self, const char* flowControl){
  self->packer->prepareFlowControl(self->packer, flowControl);
  self->packer->pack(self->packer);
}

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

  IFloatStreamHandle scopeTimestamp = self->scope->getTimestamp(self->scope);
  self->packer->prepareTimestamp(self->packer, scopeTimestamp);

  const uint32_t timeIncrement = self->scope->getTimeIncrement(self->scope);
  self->packer->prepareTimeIncrement(self->packer, timeIncrement);

  self->packer->pack(self->packer);
}

bool Sender_transmit(SenderHandle self);

void Sender_destroy(SenderHandle self){
  free(self);
  self = NULL;
}