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

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __ByteStreamPrivateData
{
  ByteRingBufferHandle buffer;

  IByteStream iByteStream;

} ByteStreamPrivateData ;

/******************************************************************************
 Public functions
******************************************************************************/
ByteStreamHandle ByteStream_create(size_t capacity){

  ByteStreamHandle self = (ByteStreamHandle) malloc(sizeof(ByteStreamPrivateData));

  self->buffer = ByteRingBuffer_create(capacity);

  return self;
}

void ByteStream_destroy(ByteStreamHandle self){
  free(self);
  self = NULL;
}

IByteStreamHandle ByteStream_getByteStream(ByteStreamHandle self){
  return ByteRingBuffer_getByteStream(self->buffer);
}



