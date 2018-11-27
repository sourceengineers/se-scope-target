/*!****************************************************************************************************************************************
 * @file         IntRingBuffer.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/GeneralPurpose/IntRingBuffer.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __IntRingBufferPrivateData
{
  gemmi_uint* data;
  gemmi_uint* head;
  gemmi_uint* tail;
  size_t capacity;
  IIntStream stream;
} IntRingBufferPrivateData ;

/* Returns the next index of the given index */
static gemmi_uint* nextIndex(IntRingBufferHandle self, gemmi_uint* index);

/* Increments the tail index */
static bool incTail(IntRingBufferHandle self);

/* Increments the head index */
static bool incHead(IntRingBufferHandle self);

/******************************************************************************
 Private functions
******************************************************************************/
static void openStream(IIntStreamHandle gemmi_uintstream, gemmi_uint* data, const size_t capacity){
  return;
}

static bool dataIsReady(IIntStreamHandle stream){
  IntRingBufferHandle self = (IntRingBufferHandle) stream->implementer;

  if(IntRingBuffer_usedData(self) > 0) {
    return true;
  } else {
    return false;
  }
}

static const gemmi_uint readData(IIntStreamHandle stream){
  IntRingBufferHandle self = (IntRingBufferHandle) stream->implementer;

  gemmi_uint data;
  IntRingBuffer_read(self, &data, 1);

  return data;
}

static void readAll(IIntStreamHandle stream, gemmi_uint* data, const size_t length){
  IntRingBufferHandle self = (IntRingBufferHandle) stream->implementer;

  IntRingBuffer_read(self, data, length);
}

static size_t streamLength(IIntStreamHandle stream){
  IntRingBufferHandle self = (IntRingBufferHandle) stream->implementer;

  return IntRingBuffer_usedData(self);
}

static void writeData(IIntStreamHandle stream, const gemmi_uint data){
  IntRingBufferHandle self = (IntRingBufferHandle) stream->implementer;

  IntRingBuffer_write(self, &data, 1);
}

static void writeAll(IIntStreamHandle stream, const gemmi_uint* data, const size_t length){
  IntRingBufferHandle self = (IntRingBufferHandle) stream->implementer;

  IntRingBuffer_write(self, data, length);
}

static void closeStream(IIntStreamHandle stream){
  return;
}
static void flush(IIntStreamHandle stream){
  IntRingBufferHandle self = (IntRingBufferHandle) stream->implementer;

  IntRingBuffer_clear(self);
}

static gemmi_uint* nextIndex(IntRingBufferHandle self, gemmi_uint* index){
  const gemmi_uint positionRelative = ((index + 1) - self->data);
  return (positionRelative % self->capacity) + self->data;
}

static bool incTail(IntRingBufferHandle self){
  if(self->tail != self->head){
    self->tail = nextIndex(self, self->tail);
    return true;
  }
  return false;
}

static bool incHead(IntRingBufferHandle self){
  if(nextIndex(self, self->head) != self->tail){
    self->head = nextIndex(self, self->head);
    return true;
  }
  return false;
}

/******************************************************************************
 Private functions
******************************************************************************/
IntRingBufferHandle IntRingBuffer_create(size_t capacity){

  /* Allocate memory and set _private variables */
  IntRingBufferHandle self = malloc(sizeof(IntRingBufferPrivateData));

  self->stream.implementer = self;
  self->stream.open = &openStream;
  self->stream.dataIsReady = &dataIsReady;
  self->stream.readData = &readData;
  self->stream.length = &streamLength;
  self->stream.read = &readAll;
  self->stream.writeData = writeData;
  self->stream.write = writeAll;
  self->stream.close = closeStream;
  self->stream.flush = flush;

  self->capacity = capacity + 1;
  self->data = malloc(sizeof(gemmi_uint) * self->capacity);

  self->tail = self->data;
  self->head = self->data;

  return self;
}

void IntRingBuffer_destroy(IntRingBufferHandle self){
  free(self->data);
  self->data = NULL;
  free(self);
  self = NULL;
}

size_t IntRingBuffer_getCapacity(IntRingBufferHandle self){
  return self->capacity - 1;
}

size_t IntRingBuffer_freeData(IntRingBufferHandle self){
  return (size_t) (self->capacity - (IntRingBuffer_usedData(self))) - 1;
}

size_t IntRingBuffer_usedData(IntRingBufferHandle self){

  const size_t absSize = (self->head >= self->tail) ?
                            (self->head - self->tail) :
                            (self->capacity - (self->tail - self->head));

  return absSize % self->capacity;
}

void IntRingBuffer_clear(IntRingBufferHandle self){
  self->head = self->data;
  self->tail = self->data;
}

int IntRingBuffer_write(IntRingBufferHandle self, const gemmi_uint* data, const size_t length){

  if(length > IntRingBuffer_freeData(self)){
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

int IntRingBuffer_read(IntRingBufferHandle self, gemmi_uint* data, const size_t length){

  if(length > IntRingBuffer_usedData(self)){
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

IIntStreamHandle IntRingBuffer_getIntStream(IntRingBufferHandle self){
  return &self->stream;
}
