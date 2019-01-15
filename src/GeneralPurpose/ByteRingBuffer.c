/*!****************************************************************************************************************************************
 * @file         ByteRingBuffer.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/GeneralPurpose/ByteRingBuffer.h>
#include <Scope/GeneralPurpose/ByteStream.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __ByteRingBufferPrivateData
{
  IByteStream stream;
  uint8_t* data;
  uint8_t* head;
  uint8_t* tail;
  size_t capacity;
} ByteRingBufferPrivateData ;

/* Returns the next index of the given index */
static uint8_t* nextIndex(ByteRingBufferHandle self, uint8_t* index);

/* Increments the tail index */
static bool incTail(ByteRingBufferHandle self);

/* Increments the head index */
static bool incHead(ByteRingBufferHandle self);

/******************************************************************************
 Private functions
******************************************************************************/
static bool dataIsReady(IByteStreamHandle stream){
  ByteRingBufferHandle self = (ByteRingBufferHandle) stream->implementer;

  if(ByteRingBuffer_getNumberOfUsedData(self) > 0) {
    return true;
  } else {
    return false;
  }
}

static uint8_t readData(IByteStreamHandle stream){
  ByteRingBufferHandle self = (ByteRingBufferHandle) stream->implementer;

  uint8_t data;
  ByteRingBuffer_read(self, &data, 1);

  return data;
}

static void readAll(IByteStreamHandle stream, uint8_t* data, const size_t length){
  ByteRingBufferHandle self = (ByteRingBufferHandle) stream->implementer;

  ByteRingBuffer_read(self, data, length);
}

static size_t streamLength(IByteStreamHandle stream){
  ByteRingBufferHandle self = (ByteRingBufferHandle) stream->implementer;

  return ByteRingBuffer_getNumberOfUsedData(self);
}

static void writeData(IByteStreamHandle stream, const uint8_t data){
  ByteRingBufferHandle self = (ByteRingBufferHandle) stream->implementer;

  ByteRingBuffer_write(self, &data, 1);
}

static void writeAll(IByteStreamHandle stream, const uint8_t* data, const size_t length){
  ByteRingBufferHandle self = (ByteRingBufferHandle) stream->implementer;

  ByteRingBuffer_write(self, data, length);
}

static void flush(IByteStreamHandle stream){
  ByteRingBufferHandle self = (ByteRingBufferHandle) stream->implementer;

  ByteRingBuffer_clear(self);
}

static uint8_t* nextIndex(ByteRingBufferHandle self, uint8_t* index){
  const uint32_t positionRelative = ((index + 1) - self->data);
  return (positionRelative % self->capacity) + self->data;
}

static bool incTail(ByteRingBufferHandle self){
  if(self->tail != self->head){
    self->tail = nextIndex(self, self->tail);
    return true;
  }
  return false;
}

static bool incHead(ByteRingBufferHandle self){
  if(nextIndex(self, self->head) != self->tail){
    self->head = nextIndex(self, self->head);
    return true;
  }
  return false;
}

/******************************************************************************
 Private functions
******************************************************************************/
ByteRingBufferHandle ByteRingBuffer_create(size_t capacity){

  /* Allocate memory and set _private variables */
  ByteRingBufferHandle self = malloc(sizeof(ByteRingBufferPrivateData));

  self->stream.implementer = self;
  self->stream.byteIsReady = &dataIsReady;
  self->stream.readByte = &readData;
  self->stream.length = &streamLength;
  self->stream.read = &readAll;
  self->stream.writeByte = writeData;
  self->stream.write = writeAll;
  self->stream.flush = flush;

  self->capacity = capacity + 1;
  self->data = malloc(sizeof(uint8_t) * self->capacity);

  self->tail = self->data;
  self->head = self->data;

  return self;
}

void ByteRingBuffer_destroy(ByteRingBufferHandle self){
  free(self->data);
  self->data = NULL;
  free(self);
  self = NULL;
}

size_t ByteRingBuffer_getCapacity(ByteRingBufferHandle self){
  return self->capacity - 1;
}

size_t ByteRingBuffer_getNumberOfFreeData(ByteRingBufferHandle self){
  return (size_t) (self->capacity - (ByteRingBuffer_getNumberOfUsedData(self))) - 1;
}

size_t ByteRingBuffer_getNumberOfUsedData(ByteRingBufferHandle self){
  
  const size_t absSize = (self->head >= self->tail) ?
                            (self->head - self->tail) :
                            (self->capacity - (self->tail - self->head));

  return absSize % self->capacity;
}

void ByteRingBuffer_clear(ByteRingBufferHandle self){
  self->head = self->data;
  self->tail = self->data;
}

int ByteRingBuffer_write(ByteRingBufferHandle self, const uint8_t* data, const size_t length){

  if(length > ByteRingBuffer_getNumberOfFreeData(self)){
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

int ByteRingBuffer_read(ByteRingBufferHandle self, uint8_t* data, const size_t length){

  if(length > ByteRingBuffer_getNumberOfUsedData(self)){
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

IByteStreamHandle ByteRingBuffer_getIByteStream(ByteRingBufferHandle self){
  return &self->stream;
}
