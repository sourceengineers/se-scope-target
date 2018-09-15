/*!****************************************************************************************************************************************
 * @file         ring_buffer.c
 *
 * @copyright    Copyright (c) 2018 by Sonnen. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samue.schuepbach@sourceengineers.com
 *
 * @brief        TODO
 *
 *****************************************************************************************************************************************/

#include <scope/ring_buffer.h>
#include <assert.h>

/* Define public data */
struct RingBufferPrivateStruct
{
  /* _private Data */
  float *data;
  float *head, *tail;
  size_t capacity;
  float *floatStream;
};

RingBuffer* RingBuffer_create(size_t capacity){

  /* Allocate memory and set _private variables */
  RingBuffer* self = malloc(sizeof(RingBuffer));
  self->_private = malloc(sizeof(RingBufferPrivate));

  self->_private->capacity = capacity;
  self->_private->data = malloc(sizeof(float) * self->_private->capacity);
  
  self->_private->tail = self->_private->data;
  self->_private->head = self->_private->data;

  /* Set functions */
  self->getCapacity = &RingBuffer_getCapacity;
  self->freeData = &RingBuffer_freeData;
  self->usedData = &RingBuffer_usedData;
  self->write = &RingBuffer_write;
  self->read = &RingBuffer_read;
  self->clear = &RingBuffer_clear;
  self->getFloatStream = &RingBuffer_getIFloatStream;

  /* Set interface functions */
  self->iFloatStream.implementer = self;
  self->iFloatStream.getSize = &RingBuffer_IFloatStream_getSize;
  self->iFloatStream.getStream = &RingBuffer_IFloatStream_getStream;
  self->iFloatStream.open = &RingBuffer_IFloatStream_open;
  self->iFloatStream.close = &RingBuffer_IFloatStream_close;
  
  return self;
}

RingBuffer* RingBuffer_destroy(RingBuffer* self){
  free(self->_private->data);
  free(self->_private);
  free(self);
}

size_t RingBuffer_getCapacity(RingBuffer* self){
  return self->_private->capacity;
}

size_t RingBuffer_freeData(RingBuffer* self){
  return (size_t) (self->_private->capacity - (self->_private->head - self->_private->tail));
}

size_t RingBuffer_usedData(RingBuffer* self){
  return (size_t) (self->_private->head - self->_private->tail);
}

void RingBuffer_clear(RingBuffer* self){
  self->_private->head = self->_private->data;
  self->_private->tail = self->_private->data;
}

static float* RingBuffer_nextIndex(RingBuffer* self, float* index){
  return ((index + 1) - self->_private->data) % self->_private->capacity + self->_private->data;
}

static bool RingBuffer_incTail(RingBuffer* self){
  if(self->_private->tail != self->_private->head){
    self->_private->tail = RingBuffer_nextIndex(self, self->_private->tail);
    return true;
  }
  return false;
}

static bool RingBuffer_incHead(RingBuffer* self){
  if(RingBuffer_nextIndex(self, self->_private->head) != self->_private->tail){ //&& self->_private->head != self->_private->tail){
    self->_private->head = RingBuffer_nextIndex(self, self->_private->head);
    return true;
  }
  return false;
}


ssize_t RingBuffer_write(RingBuffer* self, float* data, size_t length){

  if(length > self->freeData(self)){
    return -1;
  }

  size_t i = 0;
  do {
    *(self->_private->head) = data[i++];
    /* Catch buffer overflow */
    if(RingBuffer_incHead(self) == false){
      return -1;
    }
  } while(length > i);

  return i;
}

ssize_t RingBuffer_read(RingBuffer* self, float* data, size_t length){

  if(length > self->usedData(self)){
    return -1;
  }

  size_t i = 0;
  do {
    data[i++] = *(self->_private->tail);
    /* Catch buffer overflow */
    if(RingBuffer_incTail(self) == false){
      return -1;
    }
  } while(length > i);

  return i;
}

static size_t RingBuffer_IFloatStream_getSize(IFloatStream* iFloatStream){
  RingBuffer* self = (RingBuffer*)iFloatStream->implementer;
  return self->usedData(self);
}

static void RingBuffer_IFloatStream_open(IFloatStream* iFloatStream, float* floatStream){
  RingBuffer* self = (RingBuffer*)iFloatStream->implementer;
  if (floatStream != NULL) {
    self->_private->floatStream = floatStream;
  }
}

static void RingBuffer_IFloatStream_close(IFloatStream* iFloatStream){
  RingBuffer* self = (RingBuffer*)iFloatStream->implementer;
  self->_private->floatStream = NULL;
}

static size_t RingBuffer_IFloatStream_getStream(IFloatStream* iFloatStream){
  RingBuffer* self = (RingBuffer*)iFloatStream->implementer;
  if(self->_private->floatStream == NULL){
    return -1;
  }
  const size_t usedData = self->usedData(self);
  const size_t dataRead = self->read(self, self->_private->floatStream, usedData);
  return dataRead;
}

IFloatStream* RingBuffer_getIFloatStream(RingBuffer* self){
  return &self->iFloatStream;
}
