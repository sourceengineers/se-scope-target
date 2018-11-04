/*!*****************************************************************************
 * @file         FloatRingBuffer.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of a ring buffer. Data can eithet be fetched
 *                through the read function, or through the implementation of
 *                stream interface.
 ******************************************************************************/
 
#ifndef FLOATRINGBUFFER_H_
#define FLOATRINGBUFFER_H_

#include <Scope/GeneralPurpose/IFloatStream.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <Scope/GeneralPurpose/DataTypes.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __FloatRingBufferPrivateData* FloatRingBufferHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the buffer */
FloatRingBufferHandle FloatRingBuffer_create(size_t capacity);

/* Deconstructor: Deletes the instance of the buffer */
void FloatRingBuffer_destroy(FloatRingBufferHandle self);

/* Empties the buffer 
   The capacity of the buffer will not change through this, but the free data
   will increase to be the same as the capacity */
void FloatRingBuffer_clear(FloatRingBufferHandle self);

/* Returns the capacity of the buffer */
size_t FloatRingBuffer_getCapacity(FloatRingBufferHandle self);

/* Returns the amount of free data points in the buffer */
size_t FloatRingBuffer_freeData(FloatRingBufferHandle self);

/* Returns the amount of used data points in the buffer */
size_t FloatRingBuffer_usedData(FloatRingBufferHandle self);

/* Writes the data in to the buffer. The amount of written data will be returned.
   If the write operation would let the buffer overflow, a -1 will be returned.*/
ssize_t FloatRingBuffer_write(FloatRingBufferHandle self, const float* data, const size_t length);

/* Reads newest data and writes them in the passed foat array
   The amount of safed data points will be returned.
   if the length is smaller than the amount of data to be read, the function
   will return -1 */
ssize_t FloatRingBuffer_read(FloatRingBufferHandle self, float* data, const size_t length);

/* Returns the IFloatStream interface */
IFloatStreamHandle FloatRingBuffer_getFloatStream(FloatRingBufferHandle self);


#endif
