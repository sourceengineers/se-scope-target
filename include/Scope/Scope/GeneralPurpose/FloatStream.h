/*!*****************************************************************************
 * @file         FloatStream.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the FloatStream interface The stream currently
 *               works with a simple array internally.
 *               The array will be swapped to a ringbuffer in a later version.
 *               This will enable a more dynamic and fluent usage of the stream.
 ******************************************************************************/
 
#ifndef FLOATSTREAM_H_
#define FLOATSTREAM_H_

#include <Scope/GeneralPurpose/IFloatStream.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <Scope/GeneralPurpose/IFloatStream.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __FloatStreamPrivateData* FloatStreamHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the stream */
FloatStreamHandle FloatStream_create(size_t capacity);

/* Deconstructor: Deletes the instance of the stream */
void FloatStream_destroy(FloatStreamHandle self);

/* Returns the IFloatStream interface */
IFloatStreamHandle FloatStream_getFloatStream(FloatStreamHandle self);


#endif
