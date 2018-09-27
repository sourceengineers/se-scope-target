/*!*****************************************************************************
 * @file         Channel.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
*******************************************************************************/

#include <Scope/Scope.h>

/* Define public data */
typedef struct __ScopePrivateData
{
  size_t numberOfChannels;
  ChannelHandle* channels;
  RingBufferHandle* buffers;
  TriggerHandle trigger;
  CommandFactoryHandle commandFactory;
  
  int timeIncrement;
  
  IScope iScope;
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
ScopeHandle Scope_create(size_t channelSize, size_t numberOfChannels){

  ScopeHandle self = malloc(sizeof(ScopePrivateData));
  self->iScope.implementer = self;
  self->iScope.poll = &iScopePoll;
  self->iScope.trans = &iScopeTrans;
  self->iScope.setTimeIncrement = &iScopeSetTimeIncrement;

  /* Create channels and buffers */
  self->timeIncrement = 0;
  self->channels = malloc(sizeof(ChannelHandle) * numberOfChannels);
  self->buffers = malloc(sizeof(RingBufferHandle) * numberOfChannels);
  self->numberOfChannels = numberOfChannels;
  
  for (size_t i = 0; i < numberOfChannels; i++) {
    self->buffers[i] = RingBuffer_create(channelSize);
    self->channels[i] = Channel_create(self->buffers[i]);
  }
  
  /* Create Trigger */
  self->trigger = Trigger_create();
  
  /* Create command factory */
  self->commandFactory = CommandFactory_create(&self->iScope, 
                                               self->channels, 
                                               self->numberOfChannels,
                                               self->trigger);
  
  return self;
}

void Scope_destroy(ScopeHandle self){
  free(self->buffers);
  free(self->channels);
  free(self);
}

void Scope_poll(ScopeHandle self){
  for (size_t i = 0; i < self->numberOfChannels; i++) {
    Channel_poll(self->channels[i]);
  }
}
