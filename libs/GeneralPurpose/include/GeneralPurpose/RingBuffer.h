/*!*****************************************************************************
 * @file         RingBuffer.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of a ring buffer. Data can eithet be fetched
 *                through the read function, or through the implementation of
 *                stream interface.
 ******************************************************************************/
 
#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_

#include <GeneralPurpose/IFloatStream.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __RingBufferPrivateData* RingBufferHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the buffer */
RingBufferHandle RingBuffer_create(size_t capacity);

/* Deconstructor: Deletes the instance of the buffer */
void RingBuffer_destroy(RingBufferHandle self);

/* Empties the buffer 
   The capacity of the buffer will not change through this, but the free data
   will increase to be the same as the capacity */
void RingBuffer_clear(RingBufferHandle self);

/* Returns the capacity of the buffer */
size_t RingBuffer_getCapacity(RingBufferHandle self);

/* Returns the amount of free data points in the buffer */
size_t RingBuffer_freeData(RingBufferHandle self);

/* Returns the amout of used data points in the buffer */
size_t RingBuffer_usedData(RingBufferHandle self);

/* Writes the data in to the buffer. The amount of written data will be returned.
   If the write operation would let the buffer overflow, a -1 will be returned.*/
ssize_t RingBuffer_write(RingBufferHandle self, const float* data, const size_t length);

/* Reads newest data and writes them in the passed foat array
   The amount of safed data points will be returned.
   if the length is smaller than the amount of data to be read, the function
   will return -1 */
ssize_t RingBuffer_read(RingBufferHandle self, float* data, const size_t length);

/* Returns the IFloatStream interface */
IFloatStreamHandle RingBuffer_getFloatStream(RingBufferHandle self);


#endif
