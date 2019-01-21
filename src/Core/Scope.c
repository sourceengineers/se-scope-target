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
#include <Scope/Parser/JsonParser/JsonUnpacker.h>
#include <Scope/Parser/JsonParser/JsonPacker.h>
#include <Scope/Communication/Sender.h>
#include <Scope/Communication/CommunicationFactory.h>
#include <Scope/GeneralPurpose/IntRingBuffer.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __ScopePrivateData
{
  size_t numberOfChannels;
  ChannelHandle* channels;
  TriggerHandle trigger;
  CommandParserDispatcherHandle commandParserDispatcher;

  IScope scope;

  /* Timestamping data */
  uint32_t timeIncrement;
  IntRingBufferHandle timeStamp;
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

/* Fetches all commands from the Parser */
static void fetchCommands(ScopeHandle scope, IUnpackerHandle unpacker, ICommandHandle* commands, size_t numberOfCommands);

/* Executes all fetched commands */
static void runCommands(ICommandHandle* commands, size_t numberOfCommands);

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

static void fetchCommands(ScopeHandle scope, IUnpackerHandle unpacker, ICommandHandle* commands, size_t numberOfCommands){
  char commandName[MAX_COMMAND_LENGTH];

  for (size_t i = 0; i < numberOfCommands; ++i) {
    unpacker->getNameOfCommand(unpacker, commandName, MAX_COMMAND_LENGTH, i);
    commands[i] = CommandParserDispatcher_run(scope->commandParserDispatcher, commandName); // factories sollten nicht zur laufzeit aufgerufen werden. teile der commandParserDispatcher in Parser klasse auslagern, siehe CommandParserDispatcher
  }
}

static void runCommands(ICommandHandle* commands, size_t numberOfCommands){
  for (size_t i = 0; i < numberOfCommands; ++i) {
    if(commands[i] != NULL){
      commands[i]->run(commands[i]);
    }
  }
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

static IIntStreamHandle getTimestamp(IScopeHandle scope){
  ScopeHandle self = (ScopeHandle) scope->handle;
  return IntRingBuffer_getIntStream(self->timeStamp);
}
/******************************************************************************
 Public functions
******************************************************************************/
ScopeHandle Scope_create(const size_t channelSize,
                         const size_t numberOfChannels,
                         const size_t maxNumberOfAddresses,
                         const COM_TYPE comType, // diskutieren ob das hier nicht ausserhalb des scopes besser wäre (klare layerung)
                         const TIMESTAMPING_MODE timestampingMode,
                         ScopeTransmitCallback transmitCallback){

  ScopeHandle self = malloc(sizeof(ScopePrivateData));
  self->currentTimestamp = 0;
  self->timeIncrement = 1;
  self->timestampingMode = timestampingMode;

  self->scope.handle = self;
  self->scope.poll = &scopePoll;
  self->scope.transmit = &scopeTransmit;
  self->scope.setTimeIncrement = &scopeSetTimeIncrement;
  self->scope.getTimeIncrement = &getTimeIncrement;
  self->scope.getTimestamp = &getTimestamp;
  self->scope.transmitTimestampInc = &transmitTimestampInc;
  self->scope.announce = &scopeAnnounce;
  self->scope.clear = &scopeClear;

  self->addressStorage = AddressStorage_create(maxNumberOfAddresses);
  size_t outputBufferSize = JsonPacker_calculateBufferSize(numberOfChannels, maxNumberOfAddresses, channelSize);
  size_t inputBufferSize = JsonUnpacker_calculateBufferSize();

  /* Create input and output streams */
  self->inputStream = BufferedByteStream_create(inputBufferSize);
  self->outputStream = BufferedByteStream_create(outputBufferSize);

  /* Create channels and buffers */
  self->channels = malloc(sizeof(ChannelHandle) * numberOfChannels);
  self->numberOfChannels = numberOfChannels;

  for (size_t i = 0; i < numberOfChannels; i++) {
    self->channels[i] = Channel_create(channelSize);
  }
  self->timeStamp = IntRingBuffer_create(channelSize);

  /* Create Trigger */
  self->trigger = Trigger_create();

  /* Communication section */
  /* Create validators */
  self->communicationFactory = CommunicationFactory_create();
  IComValidatorHandle communicationValidator = CommunicationFactory_getIComValidator(self->communicationFactory, comType);

  // json packer und scope trennen -> diskuteren ob neue factory die scope und json layer für layer aufbaut sinnvoll ist
  self->jsonPacker = JsonPacker_create(numberOfChannels, maxNumberOfAddresses, communicationValidator,
          BufferedByteStream_getIByteStream(self->outputStream));

  self->sender = Sender_create(JsonPacker_getIPacker(self->jsonPacker), self->channels, self->numberOfChannels,
                               self->trigger,
                               &self->scope,
                               transmitCallback,
                               self->addressStorage);

  /* Create the unpacker and receiver */
  self->jsonUnpacker = JsonUnpacker_create();
  self->receiver = Receiver_create(JsonUnpacker_getIUnpacker(self->jsonUnpacker),
                                   BufferedByteStream_getIByteStream(self->inputStream),
                                   communicationValidator,
                                   self->sender);

  /* Create command commandParserDispatcher */
  self->commandParserDispatcher = CommandParserDispatcher_create(&self->scope,
                                               self->channels,
                                               self->numberOfChannels,
                                               self->trigger,
                                               Receiver_getIUnpacker(self->receiver));

  return self;
}

void Scope_destroy(ScopeHandle self){

  for (size_t i = 0; i < self->numberOfChannels; ++i) {
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

  // hier finde ich unlogisch dass du zuerst upack auf dem konkreten receiver aufrufst um danach den unpacker der im
  // receiver vergraben ist weiter zu verwenden. hier könntest du das composite pattern sauber umsetzten so dass man
  // nur noch auf den IUnpackerHandler zugreifen muss.
  IUnpackerHandle unpacker = Receiver_getIUnpacker(self->receiver);

  size_t numberOfCommands = unpacker->getNumberOfCommands(unpacker);
  ICommandHandle commands[numberOfCommands];

  fetchCommands(self, unpacker, commands, numberOfCommands);
  runCommands(commands, numberOfCommands);
}

IByteStreamHandle Scope_getInputStream(ScopeHandle self){
  return BufferedByteStream_getIByteStream(self->inputStream);
}

//das brauchts glaube ich nicht -> entfernen (du hast das mit dem senden ja über den callback gelöst)
IByteStreamHandle Scope_getOutputStream(ScopeHandle self){
  return BufferedByteStream_getIByteStream(self->outputStream);
}

void Scope_poll(ScopeHandle self, uint32_t timeStamp){

  uint32_t prepareTimeStamp = 0;

  if(self->timestampingMode == TIMESTAMP_AUTOMATIC){
    self->currentTimestamp += self->timeIncrement;
    prepareTimeStamp = self->currentTimestamp;
  } else if(self->timestampingMode == TIMESTAMP_MANUAL){
    prepareTimeStamp = timeStamp;
  }

  IntRingBuffer_write(self->timeStamp, &prepareTimeStamp, 1);

  for (size_t i = 0; i < self->numberOfChannels; i++) {
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

  if(channelId >= self->numberOfChannels){
    return;
  }

  Channel_setPollAddress(self->channels[channelId], pollAddress, type);
}

void Scope_configureTrigger(ScopeHandle self, const float level, int edge, TRIGGER_MODE mode, uint32_t channelId){

  if(channelId >= self->numberOfChannels){
    return;
  }

  TriggerConfiguration triggerConf = {
          .level = level,
          .edge = edge,
          .mode = mode,
          .channelId = channelId,
          .stream = *Channel_getTriggerDataStream(self->channels[channelId])
  };

  Trigger_configure(self->trigger, triggerConf);
}

void Scope_configureTimestampIncrement(ScopeHandle self, uint32_t timstampIncrement){
  self->timeIncrement = timstampIncrement;
}

void Scope_setChannelRunning(ScopeHandle self, uint32_t channelId){

  if(channelId >= self->numberOfChannels){
    return;
  }

  Channel_setStateRunning(self->channels[channelId]);
}

void Scope_setChannelStopped(ScopeHandle self, uint32_t channelId){

  if(channelId >= self->numberOfChannels){
    return;
  }

  Channel_setStateStopped(self->channels[channelId]);
}

void Scope_announceAddresses(ScopeHandle self){
  Sender_addressAnnouncement(self->sender);
  Sender_transmit(self->sender);
}

void Scope_clear(ScopeHandle self){
  for (size_t i = 0; i < self->numberOfChannels; i++) {
    Channel_clear(self->channels[i]);
  }
  IntRingBuffer_clear(self->timeStamp);
  self->currentTimestamp = 0;
}

void Scope_addAnnounceAddresses(ScopeHandle self, const char* name, const void* address,
                             const DATA_TYPES type,
                             const uint32_t addressId){
  AddressStorage_addAnnounceAddress(self->addressStorage, name, address, type, addressId);
}
