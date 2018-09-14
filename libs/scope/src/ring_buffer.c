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




struct RingBufferStruct
{
  /* Private Data */
  float *data;
  float *head, *tail;
  size_t capacity;

  /* Interfaces */
  IFloatStream* iFloatStream;

  /* Public Functions */
  size_t (*getCapacity)(RingBuffer* self);
  size_t (*freeData)(RingBuffer* self);
  size_t (*usedData)(RingBuffer* self);
  ssize_t (*writeData)(RingBuffer* self, float* data, size_t length);
  ssize_t (*readData)(RingBuffer* self, float* data, size_t length);
  IFloatStream* (*getIFloatStream)(IFloatStream *self);
};


RingBuffer* RingBuffer_create(size_t capacity){

  /* Allocate memory and set private variables */
  RingBuffer* self = malloc(sizeof(RingBuffer));
  self->capacity = capacity;

  self->iFloatStream = malloc(sizeof(IFloatStream));
  self->data = malloc(sizeof(float), self->capacity);
  self->tail = self->data;
  self->head = self->data;

  /* Set functions */
  self->getCapacity = &RingBuffer_getCapacity;
  self->freeData = &RingBuffer_freeData;
  self->usedData = &RingBuffer_usedData;
  self->writeData = &RingBuffer_writeData;
  self->readData = &RingBuffer_readData;

  self->iFloatStream->getSize = &RingBuffer_IFloatStream_getSize;
  self->iFloatStream->getData = &RingBuffer_IFloatStream_getData;
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

float* incIndex(float* index){
  return ((index + 1) - self->data) % self->capacity + self->data;
}

ssize_t RingBuffer_writeData(RingBuffer* self, float* data, size_t length){

  for (size_t i = 0; i < length; i++) {
    *(self->head) = data[i];
    self->head = incIndex(self->head);
  }
  return 0;
}

ssize_t RingBuffer_readData(RingBuffer* self, float* data, size_t length){
  for (size_t i = 0; i < length; i++) {
    data[i] = *(self->tail);
    self->tail = incIndex(self->tails);
  }
  return 0;
}


size_t RingBuffer_IFloatStream_getSize(IFloatStream *self);

float RingBuffer_IFloatStream_getData(IFloatStream *self);

IFloatStream* RingBuffer_getIFloatStream(IFloatStream *self);
