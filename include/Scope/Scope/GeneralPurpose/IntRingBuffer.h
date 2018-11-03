/*!*****************************************************************************
 * @file         IntRingBuffer.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of a ring buffer. Data can eithet be fetched
 *                through the read function, or through the implementation of
 *                stream interface.
 ******************************************************************************/

#ifndef INTRINGBUFFER_H_
#define INTRINGBUFFER_H_

#include <Scope/GeneralPurpose/IIntStream.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __IntRingBufferPrivateData* IntRingBufferHandle;

/******************************************************************************
 Public functions
******************************************************************************/
/* Constructor: Creates a new instance of the buffer */
IntRingBufferHandle IntRingBuffer_create(size_t capacity);

/* Deconstructor: Deletes the instance of the buffer */
void IntRingBuffer_destroy(IntRingBufferHandle self);

/* Empties the buffer
   The capacity of the buffer will not change through this, but the free data
   will increase to be the same as the capacity */
void IntRingBuffer_clear(IntRingBufferHandle self);

/* Returns the capacity of the buffer */
size_t IntRingBuffer_getCapacity(IntRingBufferHandle self);

/* Returns the amount of free data points in the buffer */
size_t IntRingBuffer_freeData(IntRingBufferHandle self);

/* Returns the amount of used data points in the buffer */
size_t IntRingBuffer_usedData(IntRingBufferHandle self);

/* Writes the data in to the buffer. The amount of written data will be returned.
   If the write operation would let the buffer overflow, a -1 will be returned.*/
ssize_t IntRingBuffer_write(IntRingBufferHandle self, const uint32_t* data, const size_t length);

/* Reads newest data and writes them in the passed foat array
   The amount of safed data points will be returned.
   if the length is smaller than the amount of data to be read, the function
   will return -1 */
ssize_t IntRingBuffer_read(IntRingBufferHandle self, uint32_t* data, const size_t length);

/* Returns the IIntStream interface */
IIntStreamHandle IntRingBuffer_getIntStream(IntRingBufferHandle self);


#endif