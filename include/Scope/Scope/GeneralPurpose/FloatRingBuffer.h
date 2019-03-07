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

#include <Scope/GeneralPurpose/DataTypes.h>
#include <Scope/GeneralPurpose/IFloatStream.h>

#include <stddef.h>

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
size_t FloatRingBuffer_getNumberOfFreeData(FloatRingBufferHandle self);

/* Returns the amount of used data points in the buffer */
size_t FloatRingBuffer_getNumberOfUsedData(FloatRingBufferHandle self);

/* Writes the data in to the buffer. The amount of written data will be returned.
   If the write operation would let the buffer overflow, a -1 will be returned.*/
int FloatRingBuffer_write(FloatRingBufferHandle self, const float* data, const size_t length);

/* Reads newest data and writes them in the passed float array
   The amount of safed data points will be returned.
   if the length is smaller than the amount of data to be read, the function
   will return -1 */
int FloatRingBuffer_read(FloatRingBufferHandle self, float* data, const size_t length);

/* Same as a normal read operation, but resets the tail pointer to its original position.
 * therefore data can be read without being destroyed. */
int FloatRingBuffer_readNoPosInc(FloatRingBufferHandle self, float* data, const size_t length);


#endif
