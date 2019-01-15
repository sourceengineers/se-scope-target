/*!****************************************************************************************************************************************
 * @file         IntStream.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/
// siehe kommmentar in ByteStream
#include <Scope/GeneralPurpose/IntStream.h>
#include <Scope/GeneralPurpose/IntRingBuffer.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __IntStreamPrivateData
{
  IntRingBufferHandle buffer;

  IIntStream iIntStream;

} IntStreamPrivateData ;

/******************************************************************************
 Public functions
******************************************************************************/
IntStreamHandle IntStream_create(size_t capacity){

  IntStreamHandle self = (IntStreamHandle) malloc(sizeof(IntStreamPrivateData));

  self->buffer = IntRingBuffer_create(capacity);
  self->iIntStream = *IntRingBuffer_getIntStream(self->buffer);

  return self;
}

void IntStream_destroy(IntStreamHandle self){
  free(self);
  self = NULL;
}

IIntStreamHandle IntStream_getIIntStream(IntStreamHandle self){
  return IntRingBuffer_getIntStream(self->buffer);
}

void IntStream_flush(IntStreamHandle self){
  self->iIntStream.flush(&self->iIntStream);
}

size_t IntStream_length(IntStreamHandle self){
  return self->iIntStream.length(&self->iIntStream);
}

void IntStream_write(IntStreamHandle self, const uint32_t* data, const size_t length){
  self->iIntStream.write(&self->iIntStream, data, length);
}

void IntStream_writeData(IntStreamHandle self, const uint32_t data){
  self->iIntStream.writeData(&self->iIntStream, data);
}

void IntStream_read(IntStreamHandle self, uint32_t* data, const size_t length){
  self->iIntStream.read(&self->iIntStream, data, length);
}

uint32_t IntStream_readData(IntStreamHandle self){
  return self->iIntStream.readData(&self->iIntStream);
}

bool IntStream_dataIsReady(IntStreamHandle self){
  return self->iIntStream.dataIsReady(&self->iIntStream);
}