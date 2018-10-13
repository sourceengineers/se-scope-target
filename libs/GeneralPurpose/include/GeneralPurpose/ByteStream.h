/*!*****************************************************************************
 * @file         ByteStream.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the ByteStream interface The stream currently
 *               works with a simple array internally.
 *               The array will be swapped to a ringbuffer in a later version.
 *               This will enable a more dynamic and fluent usage of the stream.
 ******************************************************************************/
 
#ifndef BYTESTREAM_H_
#define BYTESTREAM_H_

#include <GeneralPurpose/IFloatStream.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <IByteStream.h>


/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __ByteStreamPrivateData* ByteStreamHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the stream */
ByteStreamHandle ByteStream_create(size_t capacity);

/* Deconstructor: Deletes the instance of the stream */
void ByteStream_destroy(ByteStreamHandle self);

/* Returns the IByteStream interface */
IByteStreamHandle ByteStream_getByteStream(ByteStreamHandle self);


#endif
