/*!*****************************************************************************
 * @file         Scope.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
*******************************************************************************/

#include <Scope/Scope.h>
#include <Communication/Receiver.h>
#include <MsgpackParser/MsgpackUnpacker.h>
#include <MsgpackParser/MsgpackPacker.h>
#include <Communication/Sender.h>
#include <CommunicationFactory.h>

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

static void iScopeSetTimeIncrement(IScopeHandle self, uint32_t timeIncrement){
  ScopeHandle scope = (ScopeHandle) self->implementer;
  scope->timeIncrement = timeIncrement;
}

static void iScopeTransmit(IScopeHandle self){
  ScopeHandle scope = (ScopeHandle) self->implementer;
  Sender_scopeData(scope->sender);
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
ScopeHandle Scope_create(size_t channelSize, size_t numberOfChannels, COM_TYPE comType, TIMESTAMPING_MODE timestampingMode){

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

  /* Calculates size needed for the output communication buffer */
  /* communicationBufferSize = (numberOfChannels + timestampBuffer) * channelSize + constantProtocolSize */
  const size_t outputBufferSize = (numberOfChannels + 1) * channelSize + 200;
  /* The input buffer is not dependent of the buffer sizes, but on how many channels are in use */
  /* Each channel uses max. 50 bytes of data in the input protocol. Everything not dependent on the channels
   * will be of constant size */
  const size_t inputBufferSize = numberOfChannels * 50 + 400;

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
  self->msgpackPacker = MsgpackPacker_create(outputBufferSize, self->numberOfChannels,
                                             ByteStream_getByteStream(self->outputStream),
                                             communicationValidator);
  self->sender = Sender_create(MsgpackPacker_getIPacker(self->msgpackPacker), self->channels, self->numberOfChannels,
                               comType,
                               self->trigger,
                               &self->iScope);

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

void Scope_packMessage(ScopeHandle self){
  Sender_pack(self->sender);
  Trigger_release(self->trigger);
}

ChannelHandle Scope_test(ScopeHandle self, int index){
  return self->channels[index];
}
