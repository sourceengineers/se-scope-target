/*!****************************************************************************************************************************************
 * @file         Sender.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Communication/Sender.h>

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

  AddressStorageHandle addressStorage;

  ScopeTransmitCallback transmitCallback;

} SenderPrivateData ;

/******************************************************************************
 Private functions
******************************************************************************/

/******************************************************************************
 Public functions
******************************************************************************/
SenderHandle Sender_create(IPackerHandle packer, ChannelHandle* channels, const size_t numberOfChannels,
                           TriggerHandle trigger,
                           IScopeHandle scope,
                           ScopeTransmitCallback transmitCallback,
                           AddressStorageHandle addressStorage){

  SenderHandle self = (SenderHandle) malloc(sizeof(SenderPrivateData));

  self->packer = packer;
  self->trigger = trigger;
  self->scope = scope;
  self->addressStorage = addressStorage;

  self->channels = channels;
  self->numberOfChannels = numberOfChannels;

  self->transmitCallback = transmitCallback;

  return self;
}

void Sender_flowControl(SenderHandle self, const char* flowControl){
  self->packer->prepareFlowControl(self->packer, flowControl);
  self->packer->pack(self->packer);
}

void Sender_addressAnnouncement(SenderHandle self){

  const size_t maxAddresses = AddressStorage_getMaxAmountOfAddresses(self->addressStorage);

  for (gemmi_uint i = 0; i < maxAddresses; ++i) {
    AddressDefinition* addr = AddressStorage_getAddressToTransmit(self->addressStorage, i);
    if(addr->hasToBeSent == true){
      self->packer->prepareAddressAnnouncement(self->packer, addr->name, getDataTypeName(addr->type), addr->address);
    }
  }
  self->packer->pack(self->packer);
}

void Sender_scopeData(SenderHandle self){

  for (size_t i = 0; i < self->numberOfChannels; ++i) {
    if(Channel_isRunning(self->channels[i]) == true){
      self->packer->prepareChannel(self->packer, Channel_getRingBufferFloatStream(self->channels[i]), (const gemmi_uint) i);
    }
  }

  const bool isTriggered = Trigger_isTriggered(self->trigger);
  const gemmi_uint channelId = Trigger_getChannelId(self->trigger);
  const gemmi_uint triggerTimestamp = Trigger_getTriggerIndex(self->trigger);

  self->packer->prepareTrigger(self->packer, isTriggered, channelId, triggerTimestamp);

  IIntStreamHandle scopeTimestamp = self->scope->getTimestamp(self->scope);
  self->packer->prepareTimestamp(self->packer, scopeTimestamp);

  if(self->scope->transmitTimestampInc(self->scope) == true){
    const gemmi_uint timeIncrement = self->scope->getTimeIncrement(self->scope);
    self->packer->prepareTimeIncrement(self->packer, timeIncrement);
  }

  self->packer->pack(self->packer);
}

bool Sender_transmit(SenderHandle self){

  if(self->transmitCallback == NULL){
    return false;
  }

  IByteStreamHandle stream = self->packer->getByteStream(self->packer);
  self->transmitCallback(stream);

  return true;
}

void Sender_destroy(SenderHandle self){
  free(self);
  self = NULL;
}
