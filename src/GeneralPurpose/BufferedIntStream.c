/*!****************************************************************************************************************************************
 * @file         BufferedIntStream.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/GeneralPurpose/BufferedIntStream.h>
#include <Scope/GeneralPurpose/IntRingBuffer.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __BufferedIntStreamPrivateData {
    IIntStream parent;
    IntRingBufferHandle buffer;
} BufferedIntStreamPrivateData;

/******************************************************************************
 Public functions
******************************************************************************/
static bool dataIsReady(IIntStreamHandle stream){
  BufferedIntStreamHandle self = (BufferedIntStreamHandle) stream->implementer;

  if(IntRingBuffer_getNumberOfUsedData(self->buffer) > 0) {
    return true;
  } else {
    return false;
  }
}

static uint32_t readData(IIntStreamHandle stream){
  BufferedIntStreamHandle self = (BufferedIntStreamHandle) stream->implementer;

  uint32_t data;
  IntRingBuffer_read(self->buffer, &data, 1);

  return data;
}

static void readAll(IIntStreamHandle stream, uint32_t* data, const size_t length){
  BufferedIntStreamHandle self = (BufferedIntStreamHandle) stream->implementer;

  IntRingBuffer_read(self->buffer, data, length);
}

static size_t streamLength(IIntStreamHandle stream){
  BufferedIntStreamHandle self = (BufferedIntStreamHandle) stream->implementer;

  return IntRingBuffer_getNumberOfUsedData(self->buffer);
}

static void writeData(IIntStreamHandle stream, const uint32_t data){
  BufferedIntStreamHandle self = (BufferedIntStreamHandle) stream->implementer;

  IntRingBuffer_write(self->buffer, &data, 1);
}

static void writeAll(IIntStreamHandle stream, const uint32_t* data, const size_t length){
  BufferedIntStreamHandle self = (BufferedIntStreamHandle) stream->implementer;

  IntRingBuffer_write(self->buffer, data, length);
}

static void flush(IIntStreamHandle stream){
  BufferedIntStreamHandle self = (BufferedIntStreamHandle) stream->implementer;

  IntRingBuffer_clear(self->buffer);
}

BufferedIntStreamHandle BufferedIntStream_create(size_t capacity) {

  BufferedIntStreamHandle self = (BufferedIntStreamHandle) malloc(sizeof(BufferedIntStreamPrivateData));

  self->buffer = IntRingBuffer_create(capacity);

  self->parent.implementer = self;
  self->parent.dataIsReady = &dataIsReady;
  self->parent.readData = &readData;
  self->parent.length = &streamLength;
  self->parent.read = &readAll;
  self->parent.writeData = writeData;
  self->parent.write = writeAll;
  self->parent.flush = flush;

  return self;
}

void BufferedIntStream_destroy(BufferedIntStreamHandle self) {
  IntRingBuffer_destroy(self->buffer);

  free(self);
  self = NULL;
}

IIntStreamHandle BufferedIntStream_getIIntStream(BufferedIntStreamHandle self){
  return &self->parent;
}