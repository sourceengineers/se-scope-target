/*!*****************************************************************************
 * @file         ByteRingBuffer.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of a ring buffer. Data can either be fetched
 *                through the read function, or through the implementation of
 *                stream interface.
 ******************************************************************************/

#ifndef BYTERINGBUFFER_H_
#define BYTERINGBUFFER_H_

#include <Scope/GeneralPurpose/DataTypes.h>
#include <Scope/GeneralPurpose/IByteStream.h>

#include <stddef.h>
#include <stdint.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __ByteRingBufferPrivateData* ByteRingBufferHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the buffer */
ByteRingBufferHandle ByteRingBuffer_create(size_t capacity);

/* Deconstructor: Deletes the instance of the buffer */
void ByteRingBuffer_destroy(ByteRingBufferHandle self);

/* Empties the buffer 
   The capacity of the buffer will not change through this, but the free data
   will increase to be the same as the capacity */
void ByteRingBuffer_clear(ByteRingBufferHandle self);

/* Returns the capacity of the buffer */
size_t ByteRingBuffer_getCapacity(ByteRingBufferHandle self);

/* Returns the amount of free data points in the buffer */
// name ändern. könnte man mit dem befehl die daten zu löschen verwechseln. zb getNumberOfFreeBytes
size_t ByteRingBuffer_getNumberOfFreeData(ByteRingBufferHandle self);

/* Returns the amount of used data points in the buffer */
// name ändern, z.b. getNumberOfUsedBytes (damit es zur änderung oben pass)
size_t ByteRingBuffer_getNumberOfUsedData(ByteRingBufferHandle self);

/* Writes the data in to the buffer. The amount of written data will be returned.
   If the write operation would let the buffer overflow, a -1 will be returned.*/
int ByteRingBuffer_write(ByteRingBufferHandle self, const uint8_t* data, const size_t length);

/* Reads newest data and writes them in the passed byte array
   The amount of saved data points will be returned.
   if the length is smaller than the amount of data to be read, the function
   will return -1 */
int ByteRingBuffer_read(ByteRingBufferHandle self, uint8_t* data, const size_t length);

/* Same as a normal read operation, but resets the tail pointer to its original position.
 * therefore data can be read without being destroyed. */
int ByteRingBuffer_readNoPosInc(ByteRingBufferHandle self, uint8_t* data, const size_t length);

#endif
