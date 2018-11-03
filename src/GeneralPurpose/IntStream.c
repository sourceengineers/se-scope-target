/*!****************************************************************************************************************************************
 * @file         IntStream.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

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

  return self;
}

void IntStream_destroy(IntStreamHandle self){
  free(self);
  self = NULL;
}

IIntStreamHandle IntStream_getIntStream(IntStreamHandle self){
  return IntRingBuffer_getIntStream(self->buffer);
}
