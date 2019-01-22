/*!*****************************************************************************
 * @file         Scope.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
*******************************************************************************/

#include <Scope/Core/Scope.h>
#include <Scope/Communication/Receiver.h>
#include <Scope/Serialisation/JsonParser/JsonUnpacker.h>
#include <Scope/Serialisation/JsonParser/JsonPacker.h>
#include <Scope/Communication/Sender.h>
#include <Scope/Communication/CommunicationFactory.h>
#include <Scope/Core/ScopeTypes.h>
#include <Scope/GeneralPurpose/BufferedIntStream.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __ScopePrivateData
{
  size_t amountOfChannels;
  ChannelHandle* channels;
  TriggerHandle trigger;
  CommandParserDispatcherHandle commandParserDispatcher;

  IScope scope;

  /* Timestamping data */
  uint32_t timeIncrement;
  BufferedIntStreamHandle timeStamp;
  TIMESTAMPING_MODE timestampingMode;
  uint32_t currentTimestamp;

  /* Communication validators */
  CommunicationFactoryHandle communicationFactory;

  /* Recieving part */
  JsonUnpackerHandle jsonUnpacker;
  ReceiverHandle receiver;

  /* Sending part */
  JsonPackerHandle jsonPacker;
  SenderHandle sender;

  /* Streams */
  BufferedByteStreamHandle inputStream;
  BufferedByteStreamHandle outputStream;

  AddressStorageHandle addressStorage;

} ScopePrivateData ;


/******************************************************************************
 Private functions
******************************************************************************/
static void scopePoll(IScopeHandle scope, uint32_t timeStamp){
  ScopeHandle self = (ScopeHandle) scope->handle;
  Scope_poll(self, timeStamp);
}

static void scopeClear(IScopeHandle scope){
  ScopeHandle self = (ScopeHandle) scope->handle;
  Scope_clear(self);
}

static void scopeAnnounce(IScopeHandle scope){
  ScopeHandle self = (ScopeHandle) scope->handle;
  Scope_announceAddresses(self);
}

static void scopeSetTimeIncrement(IScopeHandle scope, uint32_t timeIncrement){
  ScopeHandle self = (ScopeHandle) scope->handle;
  Scope_configureTimestampIncrement(self, timeIncrement);
}

static void scopeTransmit(IScopeHandle scope){
  ScopeHandle self = (ScopeHandle) scope->handle;
  Sender_scopeData(self->sender);
  Sender_transmit(self->sender);
}

static uint32_t getTimeIncrement(IScopeHandle scope){
  ScopeHandle self = (ScopeHandle) scope->handle;
  return self->timeIncrement;
}

static bool transmitTimestampInc(IScopeHandle scope){
  ScopeHandle self = (ScopeHandle) scope->handle;

  if(self->timestampingMode == TIMESTAMP_AUTOMATIC){
    return true;
  } else if(self->timestampingMode == TIMESTAMP_MANUAL) {
    return false;
  }
  return true;
}

static size_t getAmountOfChannels(IScopeHandle scope){
  ScopeHandle self = (ScopeHandle) scope->handle;
  return self->amountOfChannels;
}

static IIntStreamHandle getTimestamp(IScopeHandle scope){
  ScopeHandle self = (ScopeHandle) scope->handle;
  return BufferedIntStream_getIIntStream(self->timeStamp);
}

static void configureTrigger(IScopeHandle scope, TriggerConfiguration conf){
  ScopeHandle self = (ScopeHandle) scope->handle;
  Trigger_configure(self->trigger, conf);
}

static void setChannelRunning(IScopeHandle scope, uint32_t idOfChangedChannel){
  ScopeHandle self = (ScopeHandle) scope->handle;
  Scope_setChannelRunning(self, idOfChangedChannel);
}

static void setChannelStopped(IScopeHandle scope, uint32_t idOfChangedChannel){
  ScopeHandle self = (ScopeHandle) scope->handle;
  Scope_setChannelStopped(self, idOfChangedChannel);
}

static void configureChannelAddress(IScopeHandle scope, void* address,
	                                uint32_t idOfChangedChannel, DATA_TYPES typeOfAddress){

  ScopeHandle self = (ScopeHandle) scope->handle;
  Scope_configureChannel(self, idOfChangedChannel, address, typeOfAddress);
}
/******************************************************************************
 Public functions
******************************************************************************/
ScopeHandle Scope_create(const size_t channelSize,
                         const size_t amountOfChannels,
                         const size_t maxNumberOfAddresses){
/*
  ScopeHandle self = malloc(sizeof(ScopePrivateData));
  self->currentTimestamp = 0;
  self->timeIncrement = 1;
 // self->timestampingMode = timestampingMode;

  self->scope.handle = self;
  self->scope.poll = &scopePoll;
  self->scope.transmit = &scopeTransmit;
  self->scope.setTimeIncrement = &scopeSetTimeIncrement;
  self->scope.getTimeIncrement = &getTimeIncrement;
  self->scope.getTimestamp = &getTimestamp;
  self->scope.transmitTimestampInc = &transmitTimestampInc;
  self->scope.announce = &scopeAnnounce;
  self->scope.clear = &scopeClear;
  self->scope.getAmountOfChannels = &getAmountOfChannels;
  self->scope.setChannelStopped = &setChannelStopped;
  self->scope.setChannelRunning = &setChannelRunning;
  self->scope.configureChannelAddress = &configureChannelAddress;
  self->scope.configureTrigger = &configureTrigger;

  self->addressStorage = AddressStorage_create(maxNumberOfAddresses);
  size_t outputBufferSize = JsonPacker_calculateBufferSize(amountOfChannels, maxNumberOfAddresses, channelSize);
  size_t inputBufferSize = JsonUnpacker_calculateBufferSize();

  self->inputStream = BufferedByteStream_create(inputBufferSize);
  self->outputStream = BufferedByteStream_create(outputBufferSize);

  self->channels = malloc(sizeof(ChannelHandle) * amountOfChannels);
  self->amountOfChannels = amountOfChannels;

  for (size_t i = 0; i < amountOfChannels; i++) {
    self->channels[i] = Channel_create(channelSize);
  }
  self->timeStamp = BufferedIntStream_create(channelSize);

  self->trigger = Trigger_create(self->channels, self->amountOfChannels);

  self->communicationFactory = CommunicationFactory_create();
  IComValidatorHandle communicationValidator = CommunicationFactory_getIComValidator(self->communicationFactory, comType);

  self->jsonPacker = JsonPacker_create(amountOfChannels, maxNumberOfAddresses, communicationValidator,
          BufferedByteStream_getIByteStream(self->outputStream));

  self->sender = Sender_create(JsonPacker_getIPacker(self->jsonPacker), self->channels, self->amountOfChannels,
                               self->trigger,
                               &self->scope,
                               transmitCallback,
                               self->addressStorage);

  self->jsonUnpacker = JsonUnpacker_create();
  self->receiver = Receiver_create(JsonUnpacker_getIUnpacker(self->jsonUnpacker),
                                   BufferedByteStream_getIByteStream(self->inputStream),
                                   communicationValidator,
                                   self->sender);

  self->commandParserDispatcher = CommandParserDispatcher_create(&self->scope, Receiver_getIUnpacker(self->receiver));

  return self;*/
}

void Scope_destroy(ScopeHandle self){

  for (size_t i = 0; i < self->amountOfChannels; ++i) {
    Channel_destroy(self->channels[i]);
  }

  Trigger_destroy(self->trigger);
  CommandParserDispatcher_destroy(self->commandParserDispatcher);
  JsonUnpacker_destroy(self->jsonUnpacker);
  Receiver_destroy(self->receiver);
  BufferedByteStream_destroy(self->inputStream);
  BufferedByteStream_destroy(self->outputStream);
  JsonPacker_destroy(self->jsonPacker);
  Sender_destroy(self->sender);
  CommunicationFactory_destroy(self->communicationFactory);
  AddressStorage_destroy(self->addressStorage);

  free(self);
  self = NULL;
}

void Scope_receiveData(ScopeHandle self){
 

  IByteStreamHandle stream = BufferedByteStream_getIByteStream(self->inputStream); 
  if(stream->length(stream) <= 0){
    return;
  }

  if(Receiver_unpack(self->receiver) == false){
    return;
  }
}

IByteStreamHandle Scope_getInputStream(ScopeHandle self){
  return BufferedByteStream_getIByteStream(self->inputStream);
}

void Scope_poll(ScopeHandle self, uint32_t timeStamp){

  uint32_t prepareTimeStamp = 0;

  if(self->timestampingMode == TIMESTAMP_AUTOMATIC){
    self->currentTimestamp += self->timeIncrement;
    prepareTimeStamp = self->currentTimestamp;
  } else if(self->timestampingMode == TIMESTAMP_MANUAL){
    prepareTimeStamp = timeStamp;
  }

  IIntStreamHandle stream = BufferedIntStream_getIIntStream(self->timeStamp);
  stream->write(stream, &prepareTimeStamp, 1);

  for (size_t i = 0; i < self->amountOfChannels; i++) {
    Channel_poll(self->channels[i]);
  }

  // hier ist mir unklar wie das zusammenspiel von channel und trigger und time-stamp funktioniert
  // das müssen wir diskutieren
  Trigger_run(self->trigger, prepareTimeStamp);
}

void Scope_transmitData(ScopeHandle self){
  Sender_scopeData(self->sender);
  Sender_transmit(self->sender);
  Trigger_release(self->trigger);
}

void Scope_configureChannel(ScopeHandle self, const size_t channelId, void* pollAddress, DATA_TYPES type){

  if(channelId >= self->amountOfChannels){
    return;
  }

  Channel_setPollAddress(self->channels[channelId], pollAddress, type);
}

void Scope_configureTrigger(ScopeHandle self, const float level, int edge, TRIGGER_MODE mode, uint32_t channelId){

  if(channelId >= self->amountOfChannels){
    return;
  }

  TriggerConfiguration triggerConf = {
          .level = level,
          .edge = edge,
          .mode = mode,
          .channelId = channelId,
  };

  Trigger_configure(self->trigger, triggerConf);
}

void Scope_configureTimestampIncrement(ScopeHandle self, uint32_t timstampIncrement){
  self->timeIncrement = timstampIncrement;
}

void Scope_setChannelRunning(ScopeHandle self, uint32_t channelId){

  if(channelId >= self->amountOfChannels){
    return;
  }

  Channel_setStateRunning(self->channels[channelId]);
}

void Scope_setChannelStopped(ScopeHandle self, uint32_t channelId){

  if(channelId >= self->amountOfChannels){
    return;
  }

  Channel_setStateStopped(self->channels[channelId]);
}

void Scope_announceAddresses(ScopeHandle self){
  Sender_addressAnnouncement(self->sender);
  Sender_transmit(self->sender);
}

void Scope_clear(ScopeHandle self){
  for (size_t i = 0; i < self->amountOfChannels; i++) {
    Channel_clear(self->channels[i]);
  }
  IIntStreamHandle stream = BufferedIntStream_getIIntStream(self->timeStamp);
  stream->flush(stream);
  self->currentTimestamp = 0;
}

void Scope_addAnnounceAddresses(ScopeHandle self, const char* name, const void* address,
                             const DATA_TYPES type,
                             const uint32_t addressId){
  AddressStorage_addAnnounceAddress(self->addressStorage, name, address, type, addressId);
}
