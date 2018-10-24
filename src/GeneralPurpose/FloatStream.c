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

  return self;
}

void FloatStream_destroy(FloatStreamHandle self){
  free(self);
  self = NULL;
}

IFloatStreamHandle FloatStream_getFloatStream(FloatStreamHandle self){
  return FloatRingBuffer_getFloatStream(self->buffer);
}



