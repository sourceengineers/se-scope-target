/*!****************************************************************************************************************************************
 * @file         RingBuffer.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <GeneralPurpose/RingBuffer.h>

typedef struct __RingBufferPrivateData
{
  float* data;
  float* head;
  float* tail;
  size_t capacity;
  float* floatStream;
  IFloatStream iFloatStream;
} RingBufferPrivateData ;


/* Functions for the IFloatStream interface */
static size_t streamGetSize(IFloatStreamHandle iFloatStream){
  RingBufferHandle self = (RingBufferHandle)iFloatStream->implementer;
  return RingBuffer_usedData(self);
}

static void streamOpen(IFloatStreamHandle iFloatStream, float* floatStream){
  RingBufferHandle self = (RingBufferHandle)iFloatStream->implementer;
  if (floatStream != NULL) {
    self->floatStream = floatStream;
  }
}

static void streamClose(IFloatStreamHandle iFloatStream){
  RingBufferHandle self = (RingBufferHandle)iFloatStream->implementer;
  self->floatStream = NULL;
}

static size_t streamGetData(IFloatStreamHandle iFloatStream){
  RingBufferHandle self = (RingBufferHandle)iFloatStream->implementer;
  if(self->floatStream == NULL){
    return -1;
  }
  const size_t usedData = RingBuffer_usedData(self);
  const size_t dataRead = RingBuffer_read(self, self->floatStream, usedData);
  return dataRead;
}

// private functions
static float* nextIndex(RingBufferHandle self, float* index){
  const long positionRelative = ((index + 1) - self->data);
  return (positionRelative % self->capacity) + self->data;
}

static bool incTail(RingBufferHandle self){
  if(self->tail != self->head){
    self->tail = nextIndex(self, self->tail);
    return true;
  }
  return false;
}

static bool incHead(RingBufferHandle self){
  if(nextIndex(self, self->head) != self->tail){ 
    self->head = nextIndex(self, self->head);
    return true;
  }
  return false;
}

// public functions
RingBufferHandle RingBuffer_create(size_t capacity){

  /* Allocate memory and set _private variables */
  RingBufferHandle self = malloc(sizeof(RingBufferPrivateData));
 
  self->capacity = capacity;
  self->data = malloc(sizeof(float) * self->capacity);
  
  self->tail = self->data;
  self->head = self->data;

  /* Set interface functions */
  self->iFloatStream.implementer = self;
  
  self->iFloatStream.getSize = &streamGetSize;
  self->iFloatStream.getStream = &streamGetData;
  self->iFloatStream.open = &streamOpen;
  self->iFloatStream.close = &streamClose;
  
  return self;
}

void RingBuffer_destroy(RingBufferHandle self){
  free(self->data);
  self->data = NULL;
  free(self);
  self = NULL;
}

size_t RingBuffer_getCapacity(RingBufferHandle self){
  return self->capacity;
}

size_t RingBuffer_freeData(RingBufferHandle self){
  return (size_t) (self->capacity - (RingBuffer_usedData(self)));
}

size_t RingBuffer_usedData(RingBufferHandle self){
  return (size_t) (self->head - self->tail);
}

void RingBuffer_clear(RingBufferHandle self){
  self->head = self->data;
  self->tail = self->data;
}

ssize_t RingBuffer_write(RingBufferHandle self, const float* data, const size_t length){

  if(length > RingBuffer_freeData(self)){
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

ssize_t RingBuffer_read(RingBufferHandle self, float* data, const size_t length){

  if(length > RingBuffer_usedData(self)){
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

IFloatStreamHandle RingBuffer_getFloatStream(RingBufferHandle self){
  return &self->iFloatStream;
}
