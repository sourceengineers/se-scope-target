/*!****************************************************************************************************************************************
 * @file         FloatStream.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/GeneralPurpose/FloatStream.h>
#include <Scope/GeneralPurpose/FloatRingBuffer.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __FloatStreamPrivateData
{
  FloatRingBufferHandle buffer;

  IFloatStream iFloatStream;

} FloatStreamPrivateData ;

/******************************************************************************
 Public functions
******************************************************************************/
FloatStreamHandle FloatStream_create(size_t capacity){

  FloatStreamHandle self = (FloatStreamHandle) malloc(sizeof(FloatStreamPrivateData));

  self->buffer = FloatRingBuffer_create(capacity);
  self->iFloatStream = *FloatRingBuffer_getIFloatStream(self->buffer);

  return self;
}

void FloatStream_destroy(FloatStreamHandle self){
  free(self);
  self = NULL;
}

IFloatStreamHandle FloatStream_getIFloatStream(FloatStreamHandle self){
  return FloatRingBuffer_getIFloatStream(self->buffer);
}

void FloatStream_flush(FloatStreamHandle self){
  self->iFloatStream.flush(&self->iFloatStream);
}

size_t FloatStream_length(FloatStreamHandle self){
  return self->iFloatStream.length(&self->iFloatStream);
}

void FloatStream_write(FloatStreamHandle self, const float* data, const size_t length){
  self->iFloatStream.write(&self->iFloatStream, data, length);
}

void FloatStream_writeData(FloatStreamHandle self, const float data){
  self->iFloatStream.writeData(&self->iFloatStream, data);
}

void FloatStream_read(FloatStreamHandle self, float* data, const size_t length){
  self->iFloatStream.read(&self->iFloatStream, data, length);
}

float FloatStream_readData(FloatStreamHandle self){
  return self->iFloatStream.readData(&self->iFloatStream);
}

bool FloatStream_dataIsReady(FloatStreamHandle self){
  return self->iFloatStream.dataIsReady(&self->iFloatStream);
}

