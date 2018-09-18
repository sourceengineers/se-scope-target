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

typedef struct __RingBufferPrivateData
{
  float *data;
  float *head, *tail;
  size_t capacity;
  float *floatStream;
}RingBufferPrivateData;

// local functions to realize IFloatStream interface
static size_t getSize(IFloatStream* iFloatStream){
  RingBuffer* self = (RingBuffer*)iFloatStream->implementer;
  return self->usedData(self);
}

static void open(IFloatStream* iFloatStream, float* floatStream){
  RingBuffer* self = (RingBuffer*)iFloatStream->implementer;
  if (floatStream != NULL) {
    self->_private->floatStream = floatStream;
  }
}

static void close(IFloatStream* iFloatStream){
  RingBuffer* self = (RingBuffer*)iFloatStream->implementer;
  self->_private->floatStream = NULL;
}

static size_t getStream(IFloatStream* iFloatStream){
  RingBuffer* self = (RingBuffer*)iFloatStream->implementer;
  if(self->_private->floatStream == NULL){
    return -1;
  }
  const size_t usedData = self->usedData(self);
  const size_t dataRead = self->read(self, self->_private->floatStream, usedData);
  return dataRead;
}

// private functions
static float* nextIndex(RingBuffer* self, float* index){
  const long positionRelative = ((index + 1) - self->_private->data);
  return (positionRelative % self->_private->capacity) + self->_private->data;
}


static bool incTail(RingBuffer* self){
  if(self->_private->tail != self->_private->head){
    self->_private->tail = self->_private->nextIndex(self, self->_private->tail);
    return true;
  }
  return false;
}

static bool incHead(RingBuffer* self){
  if(self->_private->nextIndex(self, self->_private->head) != self->_private->tail){ 
    self->_private->head = self->_private->nextIndex(self, self->_private->head);
    return true;
  }
  return false;
}

// public functions
RingBufferHandle RingBuffer_create(size_t capacity){

  /* Allocate memory and set _private variables */
  RingBufferPrivateData* self = malloc(sizeof(RingBufferPrivateData));
 
  self->capacity = capacity;
  self>data = malloc(sizeof(float) * self->_private->capacity);
  
  self->tail = self->_private->data;
  self->head = self->_private->data;

  /* Set private functions */
  self->nextIndex = &nextIndex;
  self->incHead = &incHead;
  self->incTail = &incTail;

  /* Set interface functions */
  self->iFloatStream.implementer = &self;
  self->iFloatStream.getSize = &getSize;
  self->iFloatStream.getStream = &getStream;
  self->iFloatStream.open = &open;
  self->iFloatStream.close = &close;
  
  return self;
}

void RingBuffer_destroy(RingBuffer* self){
  free(self->_private->data);
  free(self->_private);
  free(self);
}

static size_t RingBuffer_getCapacity(RingBuffer* self){
  return self->_private->capacity;
}

static size_t RingBuffer_freeData(RingBuffer* self){
  return (size_t) (self->_private->capacity - (self->usedData(self)));
}

static size_t RingBuffer_usedData(RingBuffer* self){
  return (size_t) (self->_private->head - self->_private->tail);
}

static void RingBuffer_clear(RingBuffer* self){
  self->_private->head = self->_private->data;
  self->_private->tail = self->_private->data;
}

static ssize_t RingBuffer_write(RingBuffer* self, const float* data, const size_t length){

  if(length > self->freeData(self)){
    return -1;
  }

  size_t i = 0;
  do {
    *(self->_private->head) = data[i++];
    /* Catch buffer overflow */
    if(self->_private->incHead(self) == false){
      return -1;
    }
  } while(length > i);

  return i;
}

static ssize_t RingBuffer_read(RingBuffer* self, float* data, const size_t length){

  if(length > self->usedData(self)){
    return -1;
  }

  size_t i = 0;
  do {
    data[i++] = *(self->_private->tail);
    /* Catch buffer overflow */
    if(self->_private->incTail(self) == false){
      return -1;
    }
  } while(length > i);

  return i;
}

static IFloatStream* RingBuffer_getIFloatStream(RingBuffer* self){
  return &self->iFloatStream;
}
