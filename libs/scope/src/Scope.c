/*!*****************************************************************************
 * @file         Scope.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
*******************************************************************************/

#include <Scope/Scope.h>
#include <Communication/Reciever.h>
#include <MsgpackParser/MsgpackUnpacker.h>
#include <MsgpackParser/MsgpackPacker.h>
#include <Communication/Sender.h>

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
  
  uint32_t timeIncrement;
  uint32_t timeStamp;
  
  IScope iScope;

  /* Recieving part */
  MsgpackUnpackerHandle msgpackUnpacker;
  RecieverHandle reciever;

  /* Sending part */
  MsgpackPackerHandle msgpackPacker;
  SenderHandle sender;

  /* Streams */
  ByteStreamHandle inputStream;
  ByteStreamHandle outputStream;

} ScopePrivateData ;

/* Fetches all commands from the Parser */
static void fetchCommands(ScopeHandle scope, IUnpackerHandle unpacker, ICommandHandle* commands, size_t numberOfCommands);

/* Excecutes all fetched commands */
static void runCommands(ICommandHandle* commands, size_t numberOfCommands);

/******************************************************************************
 Private functions
******************************************************************************/
static void iScopePoll(IScopeHandle self){
  ScopeHandle scope = (ScopeHandle) self->implementer;
  Scope_poll(scope);
}

static void iScopeSetTimeIncrement(IScopeHandle self, uint32_t timeIncrement){
  ScopeHandle scope = (ScopeHandle) self->implementer;
  scope->timeIncrement = timeIncrement;
}

static void iScopeTrans(IScopeHandle self){
  ScopeHandle scope = (ScopeHandle) self->implementer;
  
  /* Trans function has to be implemented once the parser is ready */
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
static uint32_t getTimestamp(IScopeHandle self){
  ScopeHandle scope = (ScopeHandle) self->implementer;
  return scope->timeStamp;
}
/******************************************************************************
 Public functions
******************************************************************************/
ScopeHandle Scope_create(size_t channelSize, size_t numberOfChannels, size_t communicationBufferSize, COM_TYPE comType){

  ScopeHandle self = malloc(sizeof(ScopePrivateData));
  self->timeStamp = 0;
  self->timeIncrement = 0;

  self->iScope.implementer = self;
  self->iScope.poll = &iScopePoll;
  self->iScope.trans = &iScopeTrans;
  self->iScope.setTimeIncrement = &iScopeSetTimeIncrement;
  self->iScope.getTimeIncrement = &getTimeIncrement;
  self->iScope.getTimestamp = &getTimestamp;

  /* Create input and output streams */
  self->inputStream = ByteStream_create(communicationBufferSize);
  self->outputStream = ByteStream_create(communicationBufferSize);

  /* Create channels and buffers */
  self->channels = malloc(sizeof(ChannelHandle) * numberOfChannels);
  self->buffers = malloc(sizeof(FloatRingBufferHandle) * numberOfChannels);
  self->numberOfChannels = numberOfChannels;
  
  for (size_t i = 0; i < numberOfChannels; i++) {
    self->buffers[i] = FloatRingBuffer_create(channelSize);
    self->channels[i] = Channel_create(self->buffers[i]);
  }
  self->timeStampBuffer = FloatRingBuffer_create(channelSize);
  
  /* Create Trigger */
  self->trigger = Trigger_create();

  /* Creates the unpacking communication */
  self->msgpackUnpacker = MsgpackUnpacker_create(communicationBufferSize);
  self->reciever = Reciever_create(MsgpackUnpacker_getIUnpacker(self->msgpackUnpacker), comType,
                                   ByteStream_getByteStream(self->inputStream));

  self->msgpackPacker = MsgpackPacker_create(communicationBufferSize, self->numberOfChannels,
                                             ByteStream_getByteStream(self->outputStream));
  self->sender = Sender_create(MsgpackPacker_getIPacker(self->msgpackPacker), self->channels, self->numberOfChannels,
                               comType,
                               self->trigger,
                               &self->iScope);

  /* Create command factory */
  self->commandFactory = CommandFactory_create(&self->iScope, 
                                               self->channels, 
                                               self->numberOfChannels,
                                               self->trigger,
                                               Reciever_getIUnpacker(self->reciever));

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
  Reciever_destroy(self->reciever);
  ByteStream_destroy(self->inputStream);
  ByteStream_destroy(self->outputStream);
  MsgpackPacker_destroy(self->msgpackPacker);
  Sender_destroy(self->sender);

  free(self);
  self = NULL;
}

void Scope_command(ScopeHandle self){

  if(Reciever_unpack(self->reciever) == false){
    return;
  }

  IUnpackerHandle unpacker = Reciever_getIUnpacker(self->reciever);

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

void Scope_poll(ScopeHandle self){

  self->timeStamp += self->timeIncrement;

  for (size_t i = 0; i < self->numberOfChannels; i++) {
    Channel_poll(self->channels[i]);
  }

  Trigger_run(self->trigger, self->timeStamp);
}

void Scope_packMessage(ScopeHandle self){
  Sender_pack(self->sender);
  Trigger_release(self->trigger);
}

ChannelHandle Scope_test(ScopeHandle self, int index){
  return self->channels[index];
}
