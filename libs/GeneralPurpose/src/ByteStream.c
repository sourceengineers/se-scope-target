/*!****************************************************************************************************************************************
 * @file         ByteStream.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <GeneralPurpose/ByteStream.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __ByteStreamPrivateData
{
  uint8_t* data;
  /* Data used by the stream */
  size_t length;
  size_t capacity;
  IByteStream iByteStream;

} ByteStreamPrivateData ;


static void open(IByteStreamHandle iByteStream, uint8_t* stream){
  return;
}

static bool byteIsReady(IByteStreamHandle self){
  return false;
}

static const uint8_t readByte(IByteStreamHandle self){
  return 0;
}

static size_t length(IByteStreamHandle iByteStream){
  ByteStreamHandle self = (ByteStreamHandle) iByteStream->implementer;
  return self->length;
}

static const uint8_t* readStream(IByteStreamHandle iByteStream){
  ByteStreamHandle self = (ByteStreamHandle) iByteStream->implementer;
  self->length = 0;
  return self->data;
}

static void writeByte(IByteStreamHandle iByteStream, const uint8_t data){
  ByteStreamHandle self = (ByteStreamHandle) iByteStream->implementer;

  if(self->length >= self->capacity){
    return;
  }

  self->data[self->length] = data;
  self->length += 1;
}

static void writeStream(IByteStreamHandle iByteStream, const uint8_t* data, const size_t length){
  for (size_t i = 0; i < length; ++i) {
    iByteStream->writeByte(iByteStream, data[i]);
  }
}

static void closeStream(IByteStreamHandle iByteStream){
  return;
}

static void flushStream(IByteStreamHandle iByteStream){
  ByteStreamHandle self = (ByteStreamHandle) iByteStream->implementer;
  self->length = 0;
}

/******************************************************************************
 Private functions
******************************************************************************/
ByteStreamHandle ByteStream_create(size_t capacity){

  ByteStreamHandle self = (ByteStreamHandle) malloc(sizeof(ByteStreamPrivateData));
  self->data = malloc(sizeof(uint8_t) * capacity);

  self->capacity = capacity;
  self->length = 0;
  self->iByteStream.implementer = self;

  self->iByteStream.writeByte = &writeByte;
  self->iByteStream.write = &writeStream;
  self->iByteStream.open = &open;
  self->iByteStream.byteIsReady = &byteIsReady;
  self->iByteStream.close = &closeStream;
  self->iByteStream.read = &readStream;
  self->iByteStream.readByte = &readByte;
  self->iByteStream.length = &length;
  self->iByteStream.flush = &flushStream;

  return self;
}

void ByteStream_destroy(ByteStreamHandle self){
  free(self->data);
  self = NULL;
  free(self);
  self = NULL;
}

IByteStreamHandle ByteStream_getByteStream(ByteStreamHandle self){
  return &self->iByteStream;
}



