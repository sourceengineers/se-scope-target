/*!****************************************************************************************************************************************
 * @file         Channel.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Command/CommandRunning.h>

/* Define public data */
typedef struct __CommandRunningPrivateData
{
} CommandRunningPrivateData ;

/* Public functions */
CommandRunning Scope_create(size_t channelSize, size_t numberOfChannels){

  ScopeHandle self = malloc(sizeof(ScopePrivateData));
  self->channels = malloc(sizeof(ChannelHandle) * numberOfChannels);
  self->buffers = malloc(sizeof(RingBufferHandle) * numberOfChannels)
  
  for (size_t i = 0; i < numberOfChannels; i++) {
    self->buffers[i] = RingBuffer_create(channelSize);
    self->channels[i] = Channel_create(self->buffers[i]);
  }
  
  self->trigger = Trigger_create();
  
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
