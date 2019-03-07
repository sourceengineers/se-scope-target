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

#include <Scope/GeneralPurpose/DataTypes.h>
#include <Scope/GeneralPurpose/IIntStream.h>

#include <stddef.h>
#include <stdint.h>

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
size_t IntRingBuffer_getNumberOfFreeData(IntRingBufferHandle self);

/* Returns the amount of used data points in the buffer */
size_t IntRingBuffer_getNumberOfUsedData(IntRingBufferHandle self);

/* Writes the data in to the buffer. The amount of written data will be returned.
   If the write operation would let the buffer overflow, a -1 will be returned.*/
int IntRingBuffer_write(IntRingBufferHandle self, const uint32_t* data, const size_t length);

/* Reads newest data and writes them in the passed foat array
   The amount of safed data points will be returned.
   if the length is smaller than the amount of data to be read, the function
   will return -1 */
int IntRingBuffer_read(IntRingBufferHandle self, uint32_t* data, const size_t length);

/* Same as a normal read operation, but resets the tail pointer to its original position.
 * therefore data can be read without being destroyed. */
int IntRingBuffer_readNoPosInc(IntRingBufferHandle self, uint32_t* data, const size_t length);

#endif
