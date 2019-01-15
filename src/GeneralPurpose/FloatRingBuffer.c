/*!****************************************************************************************************************************************
 * @file         FloatRingBuffer.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/GeneralPurpose/FloatRingBuffer.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __FloatRingBufferPrivateData
{
  IFloatStream stream;
  float* data;
  float* head;
  float* tail;
  size_t capacity;
} FloatRingBufferPrivateData ;

/* Returns the next index of the given index */
static float* nextIndex(FloatRingBufferHandle self, float* index);

/* Increments the tail index */
static bool incTail(FloatRingBufferHandle self);

/* Increments the head index */
static bool incHead(FloatRingBufferHandle self);

/******************************************************************************
 Private functions
******************************************************************************/
static bool dataIsReady(IFloatStreamHandle stream){
  FloatRingBufferHandle self = (FloatRingBufferHandle) stream->implementer;

  if(FloatRingBuffer_getNumberOfUsedData(self) > 0) {
    return true;
  } else {
    return false;
  }
}

static float readData(IFloatStreamHandle stream){
  FloatRingBufferHandle self = (FloatRingBufferHandle) stream->implementer;

  float data;
  FloatRingBuffer_read(self, &data, 1);

  return data;
}

static void readAll(IFloatStreamHandle stream, float* data, const size_t length){
  FloatRingBufferHandle self = (FloatRingBufferHandle) stream->implementer;

  FloatRingBuffer_read(self, data, length);
}

static size_t streamLength(IFloatStreamHandle stream){
  FloatRingBufferHandle self = (FloatRingBufferHandle) stream->implementer;

  return FloatRingBuffer_getNumberOfUsedData(self);
}

static void writeData(IFloatStreamHandle stream, const float data){
  FloatRingBufferHandle self = (FloatRingBufferHandle) stream->implementer;

  FloatRingBuffer_write(self, &data, 1);
}

static void writeAll(IFloatStreamHandle stream, const float* data, const size_t length){
  FloatRingBufferHandle self = (FloatRingBufferHandle) stream->implementer;

  FloatRingBuffer_write(self, data, length);
}

static void flush(IFloatStreamHandle stream){
  FloatRingBufferHandle self = (FloatRingBufferHandle) stream->implementer;

  FloatRingBuffer_clear(self);
}

static float* nextIndex(FloatRingBufferHandle self, float* index){
  const gemmi_uint positionRelative = ((index + 1) - self->data);
  return (positionRelative % self->capacity) + self->data;
}

static bool incTail(FloatRingBufferHandle self){
  if(self->tail != self->head){
    self->tail = nextIndex(self, self->tail);
    return true;
  }
  return false;
}

static bool incHead(FloatRingBufferHandle self){
  if(nextIndex(self, self->head) != self->tail){
    self->head = nextIndex(self, self->head);
    return true;
  }
  return false;
}

/******************************************************************************
 Private functions
******************************************************************************/
FloatRingBufferHandle FloatRingBuffer_create(size_t capacity){

  /* Allocate memory and set _private variables */
  FloatRingBufferHandle self = malloc(sizeof(FloatRingBufferPrivateData));

  self->stream.implementer = self;
  self->stream.dataIsReady = &dataIsReady;
  self->stream.readData = &readData;
  self->stream.length = &streamLength;
  self->stream.read = &readAll;
  self->stream.writeData = writeData;
  self->stream.write = writeAll;
  self->stream.flush = flush;

  self->capacity = capacity + 1;
  self->data = malloc(sizeof(float) * self->capacity);

  self->tail = self->data;
  self->head = self->data;

  return self;
}

void FloatRingBuffer_destroy(FloatRingBufferHandle self){
  free(self->data);
  self->data = NULL;
  free(self);
  self = NULL;
}

size_t FloatRingBuffer_getCapacity(FloatRingBufferHandle self){
  return self->capacity - 1;
}

size_t FloatRingBuffer_getNumberOfFreeData(FloatRingBufferHandle self){
  return (size_t) (self->capacity - (FloatRingBuffer_getNumberOfUsedData(self))) - 1;
}

size_t FloatRingBuffer_getNumberOfUsedData(FloatRingBufferHandle self){
  const size_t absSize = (self->head >= self->tail) ?
                            (self->head - self->tail) :
                            (self->capacity - (self->tail - self->head));

  return absSize % self->capacity;
}

void FloatRingBuffer_clear(FloatRingBufferHandle self){
  self->head = self->data;
  self->tail = self->data;
}

int FloatRingBuffer_write(FloatRingBufferHandle self, const float* data, const size_t length){

  if(length > FloatRingBuffer_getNumberOfFreeData(self)){
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

int FloatRingBuffer_read(FloatRingBufferHandle self, float* data, const size_t length){

  if(length > FloatRingBuffer_getNumberOfUsedData(self)){
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

IFloatStreamHandle FloatRingBuffer_getIFloatStream(FloatRingBufferHandle self){
  return &self->stream;
}
