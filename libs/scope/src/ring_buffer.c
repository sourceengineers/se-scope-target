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

RingBuffer* RingBuffer_create(size_t capacity){

  /* Allocate memory and set private variables */
  RingBuffer* self = malloc(sizeof(RingBuffer));
  self->capacity = capacity;

  self->iFloatStream = malloc(sizeof(IFloatStream));
  self->data = malloc(sizeof(float) * self->capacity);
  self->tail = self->data;
  self->head = self->data;

  /* Set functions */
  self->getCapacity = &RingBuffer_getCapacity;
  self->freeData = &RingBuffer_freeData;
  self->usedData = &RingBuffer_usedData;
  self->write = &RingBuffer_writeData;
  self->read = &RingBuffer_readData;
  self->clear = &RingBuffer_clear;

  /*self->iFloatStream->getSize = &RingBuffer_IFloatStream_getSize;
  self->iFloatStream->getData = &RingBuffer_IFloatStream_getData;*/
  
  return self;
}

size_t RingBuffer_getCapacity(RingBuffer* self){
  return self->capacity;
}

size_t RingBuffer_freeData(RingBuffer* self){
  return (size_t) (self->capacity - (self->head - self->tail));
}

size_t RingBuffer_usedData(RingBuffer* self){
  return (size_t) (self->head - self->tail);
}

void RingBuffer_clear(RingBuffer* self){
  self->head = self->data;
  self->tail = self->data;
}

float* nextIndex(RingBuffer* self, float* index){
  return ((index + 1) - self->data) % self->capacity + self->data;
}

bool incTail(RingBuffer* self){
  if(/*nextIndex(self, self->tail) != self->head && */self->tail != self->head){
    self->tail = nextIndex(self, self->tail);
    return true;
  }
  return false;
}

bool incHead(RingBuffer* self){
  if(nextIndex(self, self->head) != self->tail){ //&& self->head != self->tail){
    self->head = nextIndex(self, self->head);
    return true;
  }
  return false;
}


ssize_t RingBuffer_writeData(RingBuffer* self, float* data, size_t length){

  if(length > self->freeData(self)){
    return -1;
  }

  size_t i = 0;
  do {
    *(self->head) = data[i++];
    /* Catch buffer overflow */
    if(incHead(self) == false){
      return -1;
    }
  } while(length > i);

  return i;
}

ssize_t RingBuffer_readData(RingBuffer* self, float* data, size_t length){

  if(length > self->usedData(self)){
    return -1;
  }

  size_t i = 0;
  do {
    data[i++] = *(self->tail);
    /* Catch buffer overflow */
    if(incTail(self) == false){
      return -1;
    }
  } while(length > i);

  return i;
}


size_t RingBuffer_IFloatStream_getSize(IFloatStream *self);

float RingBuffer_IFloatStream_getData(IFloatStream *self);

IFloatStream* RingBuffer_getIFloatStream(IFloatStream *self);
