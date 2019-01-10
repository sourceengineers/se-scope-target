/*!****************************************************************************************************************************************
 * @file         ByteStream.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/GeneralPurpose/ByteStream.h>
#include <Scope/GeneralPurpose/ByteRingBuffer.h>
// braucht es diese klasse überhaupt? das ist ja nur ein wrapper um den buffer. Ich würde eine der folgenden Lösungen machen
// a) klasse löschen
// a) die klasse in BufferedByteStream umbenennen, den buffer nicht von IByteStream erben. die vererbung in diese klasse verlagern

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __ByteStreamPrivateData
{
    IByteStream iByteStream; // wenn ich nur einfach-vererbung mache nenne ich das oft parent
    ByteRingBufferHandle buffer;
} ByteStreamPrivateData ;

/******************************************************************************
 Public functions
******************************************************************************/
ByteStreamHandle ByteStream_create(size_t capacity){

  ByteStreamHandle self = (ByteStreamHandle) malloc(sizeof(ByteStreamPrivateData));

  self->buffer = ByteRingBuffer_create(capacity);
  self->iByteStream = *ByteRingBuffer_getIByteStream(self->buffer);

  return self;
}

void ByteStream_destroy(ByteStreamHandle self){
  free(self);
  self = NULL;
}

IByteStreamHandle ByteStream_getIByteStream(ByteStreamHandle self){
  return ByteRingBuffer_getIByteStream(self->buffer);
}

void ByteStream_flush(ByteStreamHandle self){
  self->iByteStream.flush(&self->iByteStream);
}

size_t ByteStream_length(ByteStreamHandle self){
  return self->iByteStream.length(&self->iByteStream);
}

void ByteStream_write(ByteStreamHandle self, const uint8_t* data, const size_t length){
  self->iByteStream.write(&self->iByteStream, data, length);
}

void ByteStream_writeByte(ByteStreamHandle self, const uint8_t data){
  self->iByteStream.writeByte(&self->iByteStream, data);
}

void ByteStream_read(ByteStreamHandle self, uint8_t* data, const size_t length){
  self->iByteStream.read(&self->iByteStream, data, length);
}

uint8_t ByteStream_readData(ByteStreamHandle self){
  return self->iByteStream.readByte(&self->iByteStream);
}

bool ByteStream_dataIsReady(ByteStreamHandle self){
  return self->iByteStream.byteIsReady(&self->iByteStream);
}

