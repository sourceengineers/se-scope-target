/*!*****************************************************************************
 * @file         BufferedFloatStream.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the BufferedFloatStream interface The stream currently
 *               works with a simple array internally.
 *               The array will be swapped to a ringbuffer in a later version.
 *               This will enable a more dynamic and fluent usage of the stream.
 ******************************************************************************/

#ifndef FLOATSTREAM_H_
#define FLOATSTREAM_H_

#include <Scope/GeneralPurpose/DataTypes.h>
#include <Scope/GeneralPurpose/IFloatStream.h>

#include <stdbool.h>
#include <stddef.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __BufferedFloatStreamPrivateData* BufferedFloatStreamHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the stream */
BufferedFloatStreamHandle BufferedFloatStream_create(size_t capacity);

/* Deconstructor: Deletes the instance of the stream */
void BufferedFloatStream_destroy(BufferedFloatStreamHandle self);

/* Returns the IFloatStream interface */
IFloatStreamHandle BufferedFloatStream_getIFloatStream(BufferedFloatStreamHandle self);

/* Flushes the stream */
void BufferedFloatStream_flush(BufferedFloatStreamHandle self);

/* Returns the amount of data currently pending in the stream */
size_t BufferedFloatStream_length(BufferedFloatStreamHandle self);

/* Writes "length" amount of data into the stream */
void BufferedFloatStream_write(BufferedFloatStreamHandle self, const float* data, const size_t length);

/* Writes one single data point into the stream */
void BufferedFloatStream_writeFloat(BufferedFloatStreamHandle self, const float data);

/* Reads "length" amount of data from the stream and writes it into "data" */
void FloatStream_read(BufferedFloatStreamHandle self, float* data, const size_t length);

/* Returns one single data point */
float FloatStream_readFloat(BufferedFloatStreamHandle self);

/* Returns of a data point is ready to be read or not */
bool FloatStream_byteIsReady(BufferedFloatStreamHandle self);

#endif
