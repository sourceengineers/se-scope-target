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
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <Scope/GeneralPurpose/IFloatStream.h>
#include <Scope/GeneralPurpose/DataTypes.h>

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
IFloatStreamHandle FloatStream_getIFloatStream(FloatStreamHandle self);

/* Flushes the stream */
void FloatStream_flush(FloatStreamHandle self);

/* Returns the amount of data currently pending in the stream */
size_t FloatStream_length(FloatStreamHandle self);

/* Writes "length" amount of data into the stream */
void FloatStream_write(FloatStreamHandle self, const float* data, const size_t length);

/* Writes one single data point into the stream */
void FloatStream_writeData(FloatStreamHandle self, const float data);

/* Reads "length" amount of data from the stream and writes it into "data" */
void FloatStream_read(FloatStreamHandle self, float* data, const size_t length);

/* Returns one single data point */
float FloatStream_readData(FloatStreamHandle self);

/* Returns of a data point is ready to be read or not */
bool FloatStream_dataIsReady(FloatStreamHandle self);

#endif
