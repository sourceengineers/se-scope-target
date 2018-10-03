/*!*****************************************************************************
 * @file         Channel.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
*******************************************************************************/

#include <Scope/Scope.h>
#include <Communication/Reciever.h>
#include <MsgpackParser/MsgpackUnpacker.h>

/* Define public data */
typedef struct __ScopePrivateData
{
  size_t numberOfChannels;
  ChannelHandle* channels;
  RingBufferHandle timeStampBuffer;
  RingBufferHandle* buffers;
  TriggerHandle trigger;
  CommandFactoryHandle commandFactory;
  
  int timeIncrement;
  int timeStamp;
  
  IScope iScope;

  MsgpackUnpackerHandle msgpackUnpacker;
  RecieverHandle reciever;

} ScopePrivateData ;

static void iScopePoll(IScopeHandle self){
  ScopeHandle scope = (ScopeHandle) self->implementer;
  Scope_poll(scope);
}

static void iScopeSetTimeIncrement(IScopeHandle self, int timeIncrement){
  ScopeHandle scope = (ScopeHandle) self->implementer;
  scope->timeIncrement = timeIncrement;
}

static void iScopeTrans(IScopeHandle self){
  ScopeHandle scope = (ScopeHandle) self->implementer;
  
  /* Trans function has to be implemented once the parser is ready */
}

/* Public functions */
ScopeHandle Scope_create(size_t channelSize, size_t numberOfChannels, size_t communicationBufferSize, COM_TYPE comType){

  ScopeHandle self = malloc(sizeof(ScopePrivateData));
  self->timeStamp = 0;
  self->timeIncrement = 0;

  self->iScope.implementer = self;
  self->iScope.poll = &iScopePoll;
  self->iScope.trans = &iScopeTrans;
  self->iScope.setTimeIncrement = &iScopeSetTimeIncrement;

  /* Create channels and buffers */
  self->channels = malloc(sizeof(ChannelHandle) * numberOfChannels);
  self->buffers = malloc(sizeof(RingBufferHandle) * numberOfChannels);
  self->numberOfChannels = numberOfChannels;
  
  for (size_t i = 0; i < numberOfChannels; i++) {
    self->buffers[i] = RingBuffer_create(channelSize);
    self->channels[i] = Channel_create(self->buffers[i]);
  }
  self->timeStampBuffer = RingBuffer_create(channelSize);
  
  /* Create Trigger */
  self->trigger = Trigger_create();

  /* Creates the unpacking communication */
  self->msgpackUnpacker = MsgpackUnpacker_create(communicationBufferSize);
  self->reciever = Reciever_create(MsgpackUnpacker_getIUnpacker(self->msgpackUnpacker), comType);

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
    RingBuffer_destroy(self->buffers[i]);
    Channel_destroy(self->channels[i]);
  }
  RingBuffer_destroy(self->timeStampBuffer);

  Trigger_destroy(self->trigger);
  CommandFactory_destroy(self->commandFactory);
  MsgpackUnpacker_destroy(self->msgpackUnpacker);
  Reciever_destroy(self->reciever);

  free(self);
  self = NULL;
}

static void fetchCommands(ScopeHandle scope, IUnpackerHandle unpacker, ICommandHandle* commands, size_t numberOfCommands){
  const size_t maxCommandNameLength = 30;
  char commandName[maxCommandNameLength];

  for (size_t i = 0; i < numberOfCommands; ++i) {
    unpacker->getNameOfCommand(unpacker, commandName, maxCommandNameLength, i);
    commands[i] = CommandFactory_getICommand(scope->commandFactory, commandName);
  }
}

static void runCommands(ICommandHandle* commands, size_t numberOfCommands){
  for (size_t i = 0; i < numberOfCommands; ++i) {
    commands[i]->run(commands[i]);
  }
}

void Scope_command(ScopeHandle self, const char* data, size_t dataLength){

  IUnpackerHandle unpacker = Reciever_getIUnpacker(self->reciever);

  if(unpacker->unpack(unpacker, data, dataLength) == false){
    return;
  }

  size_t numberOfCommands = unpacker->getNumberOfCommands(unpacker);
  ICommandHandle commands[numberOfCommands];

  fetchCommands(self, unpacker, commands, numberOfCommands);
  runCommands(commands, numberOfCommands);
}

void Scope_poll(ScopeHandle self){

  RingBuffer_write(self->timeStampBuffer, (const float*) self->timeStamp, 1);

  self->timeStamp += self->timeIncrement;

  for (size_t i = 0; i < self->numberOfChannels; i++) {
    Channel_poll(self->channels[i]);
  }
}
