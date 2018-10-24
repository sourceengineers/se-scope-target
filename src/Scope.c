/*!*****************************************************************************
 * @file         Scope.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
*******************************************************************************/

#include <Scope/Scope.h>
#include <Scope/Communication/Receiver.h>
#include <Scope/MsgpackParser/MsgpackUnpacker.h>
#include <Scope/MsgpackParser/MsgpackPacker.h>
#include <Scope/Communication/Sender.h>
#include <Scope/Communication/CommunicationFactory.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __ScopePrivateData
{
  size_t numberOfChannels;
  ChannelHandle* channels;
  FloatRingBufferHandle timeStampBuffer;
  FloatRingBufferHandle* buffers;
  TriggerHandle trigger;
  CommandFactoryHandle commandFactory;

  IScope iScope;

  /* Timestamping data */
  uint32_t timeIncrement;
  FloatRingBufferHandle timeStamp;
  TIMESTAMPING_MODE timestampingMode;
  uint32_t currentTimestamp;

  /* Communication validators */
  CommunicationFactoryHandle communicationFactory;

  /* Recieving part */
  MsgpackUnpackerHandle msgpackUnpacker;
  ReceiverHandle receiver;

  /* Sending part */
  MsgpackPackerHandle msgpackPacker;
  SenderHandle sender;

  /* Streams */
  ByteStreamHandle inputStream;
  ByteStreamHandle outputStream;

  AddressStorageHandle addressStorage;

} ScopePrivateData ;

/* Fetches all commands from the Parser */
static void fetchCommands(ScopeHandle scope, IUnpackerHandle unpacker, ICommandHandle* commands, size_t numberOfCommands);

/* Executes all fetched commands */
static void runCommands(ICommandHandle* commands, size_t numberOfCommands);

/******************************************************************************
 Private functions
******************************************************************************/
static void iScopePoll(IScopeHandle self, uint32_t timeStamp){
  ScopeHandle scope = (ScopeHandle) self->implementer;
  Scope_poll(scope, timeStamp);
}

static void iScopeAnnounce(IScopeHandle self){
  ScopeHandle scope = (ScopeHandle) self->implementer;
  Scope_announceWatchAddresses(scope);
}

static void iScopeSetTimeIncrement(IScopeHandle self, uint32_t timeIncrement){
  ScopeHandle scope = (ScopeHandle) self->implementer;
  Scope_configureTimestampIncrement(scope, timeIncrement);
}

static void iScopeTransmit(IScopeHandle self){
  ScopeHandle scope = (ScopeHandle) self->implementer;
  Sender_scopeData(scope->sender);
  Sender_transmit(scope->sender);
}

static void fetchCommands(ScopeHandle scope, IUnpackerHandle unpacker, ICommandHandle* commands, size_t numberOfCommands){
  char commandName[MAX_COMMAND_LENGTH];

  for (size_t i = 0; i < numberOfCommands; ++i) {
    unpacker->getNameOfCommand(unpacker, commandName, MAX_COMMAND_LENGTH, i);
    commands[i] = CommandFactory_getICommand(scope->commandFactory, commandName);
  }
}

static void runCommands(ICommandHandle* commands, size_t numberOfCommands){
  for (size_t i = 0; i < numberOfCommands; ++i) {
    commands[i]->run(commands[i]);
  }
}

static uint32_t getTimeIncrement(IScopeHandle self){
  ScopeHandle scope = (ScopeHandle) self->implementer;
  return scope->timeIncrement;
}

static bool transmitTimestampInc(IScopeHandle self){
  ScopeHandle scope = (ScopeHandle) self->implementer;

  if(scope->timestampingMode == TIMESTAMP_AUTOMATIC){
    return true;
  } else if(scope->timestampingMode == TIMESTAMP_MANUAL) {
    return false;
  }
  return true;
}

static IFloatStreamHandle getTimestamp(IScopeHandle self){
  ScopeHandle scope = (ScopeHandle) self->implementer;
  return FloatRingBuffer_getFloatStream(scope->timeStamp);
}
/******************************************************************************
 Public functions
******************************************************************************/
ScopeHandle Scope_create(const size_t channelSize,
                         const size_t numberOfChannels,
                         const size_t maxNumberOfAddresses,
                         const COM_TYPE comType,
                         const TIMESTAMPING_MODE timestampingMode,
                         ScopeTransmitCallback transmitCallback){

  ScopeHandle self = malloc(sizeof(ScopePrivateData));
  self->currentTimestamp = 0;
  self->timeIncrement = 1;
  self->timestampingMode = timestampingMode;

  self->iScope.implementer = self;
  self->iScope.poll = &iScopePoll;
  self->iScope.transmit = &iScopeTransmit;
  self->iScope.setTimeIncrement = &iScopeSetTimeIncrement;
  self->iScope.getTimeIncrement = &getTimeIncrement;
  self->iScope.getTimestamp = &getTimestamp;
  self->iScope.transmitTimestampInc = &transmitTimestampInc;
  self->iScope.announce = &iScopeAnnounce;

  /* Calculates size needed for the output communication buffer */
  /* communicationBufferSize = (numberOfChannels + timestampBuffer) * channelSize + constantProtocolSize */
  const size_t outputBufferSize = (numberOfChannels + 1) * channelSize + 200;
  /* The input buffer is not dependent of the buffer sizes, but on how many channels are in use */
  /* Each channel uses max. 50 bytes of data in the input protocol. Everything not dependent on the channels
   * will be of constant size */
  const size_t inputBufferSize = numberOfChannels * 50 + 400;

  self->addressStorage = AddressStorage_create(maxNumberOfAddresses);

  /* Create input and output streams */
  self->inputStream = ByteStream_create(inputBufferSize);
  self->outputStream = ByteStream_create(outputBufferSize);

  /* Create channels and buffers */
  self->channels = malloc(sizeof(ChannelHandle) * numberOfChannels);
  self->buffers = malloc(sizeof(FloatRingBufferHandle) * numberOfChannels);
  self->numberOfChannels = numberOfChannels;
  
  for (size_t i = 0; i < numberOfChannels; i++) {
    self->buffers[i] = FloatRingBuffer_create(channelSize);
    self->channels[i] = Channel_create(self->buffers[i]);
  }
  self->timeStampBuffer = FloatRingBuffer_create(channelSize);
  self->timeStamp = FloatRingBuffer_create(channelSize);
  
  /* Create Trigger */
  self->trigger = Trigger_create();

  /* Communication section */
  /* Create validators */
  self->communicationFactory = CommunicationFactory_create();
  IComValidatorHandle communicationValidator = CommunicationFactory_getIComValidator(self->communicationFactory, comType);

  /* Create the sender and packer */
  self->msgpackPacker = MsgpackPacker_create(outputBufferSize, self->numberOfChannels, maxNumberOfAddresses,
                                             ByteStream_getByteStream(self->outputStream),
                                             communicationValidator);
  self->sender = Sender_create(MsgpackPacker_getIPacker(self->msgpackPacker), self->channels, self->numberOfChannels,
                               self->trigger,
                               &self->iScope,
                               transmitCallback,
                               self->addressStorage);

  /* Create the unpacker and receiver */
  self->msgpackUnpacker = MsgpackUnpacker_create(inputBufferSize);
  self->receiver = Receiver_create(MsgpackUnpacker_getIUnpacker(self->msgpackUnpacker),
                                   ByteStream_getByteStream(self->inputStream),
                                   communicationValidator,
                                   self->sender);

  /* Create command factory */
  self->commandFactory = CommandFactory_create(&self->iScope, 
                                               self->channels, 
                                               self->numberOfChannels,
                                               self->trigger,
                                               Receiver_getIUnpacker(self->receiver));

  return self;
}

void Scope_destroy(ScopeHandle self){

  for (size_t i = 0; i < self->numberOfChannels; ++i) {
    FloatRingBuffer_destroy(self->buffers[i]);
    Channel_destroy(self->channels[i]);
  }
  FloatRingBuffer_destroy(self->timeStampBuffer);

  Trigger_destroy(self->trigger);
  CommandFactory_destroy(self->commandFactory);
  MsgpackUnpacker_destroy(self->msgpackUnpacker);
  Receiver_destroy(self->receiver);
  ByteStream_destroy(self->inputStream);
  ByteStream_destroy(self->outputStream);
  MsgpackPacker_destroy(self->msgpackPacker);
  Sender_destroy(self->sender);
  CommunicationFactory_destroy(self->communicationFactory);
  AddressStorage_destroy(self->addressStorage);

  free(self);
  self = NULL;
}

void Scope_command(ScopeHandle self){

  if(Receiver_unpack(self->receiver) == false){
    return;
  }

  IUnpackerHandle unpacker = Receiver_getIUnpacker(self->receiver);

  size_t numberOfCommands = unpacker->getNumberOfCommands(unpacker);
  ICommandHandle commands[numberOfCommands];

  fetchCommands(self, unpacker, commands, numberOfCommands);
  runCommands(commands, numberOfCommands);
}

IByteStreamHandle Scope_getInputStream(ScopeHandle self){
  return ByteStream_getByteStream(self->inputStream);
}

IByteStreamHandle Scope_getOutputStream(ScopeHandle self){
  return ByteStream_getByteStream(self->outputStream);
}

void Scope_poll(ScopeHandle self, uint32_t timeStamp){

  uint32_t prepareTimeStamp = 0;

  if(self->timestampingMode == TIMESTAMP_AUTOMATIC){
    self->currentTimestamp += self->timeIncrement;
    prepareTimeStamp = self->currentTimestamp;
  } else if(self->timestampingMode == TIMESTAMP_MANUAL){
    prepareTimeStamp = timeStamp;
  }

  const float timestampToCast = (float) prepareTimeStamp;
  FloatRingBuffer_write(self->timeStamp, &timestampToCast, 1);

  for (size_t i = 0; i < self->numberOfChannels; i++) {
    Channel_poll(self->channels[i]);
  }

  Trigger_run(self->trigger, timeStamp);
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


void Scope_announceWatchAddresses(ScopeHandle self){
  Sender_addressAnnouncement(self->sender);
  Sender_transmit(self->sender);
}

void Scope_setWatchAddresses(ScopeHandle self, const char* name, const void* address,
                             const DATA_TYPES type,
                             const uint32_t addressId){
  AddressStorage_setWatchAddress(self->addressStorage, name, address, type, addressId);
}
