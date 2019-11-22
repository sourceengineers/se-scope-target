/*!*****************************************************************************
 * @file         IntRingBuffer.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of a ringbuffer for int values.
 *
 ******************************************************************************/

#ifndef INTRINGBUFFER_H_
#define INTRINGBUFFER_H_

#include "Scope/GeneralPurpose/DataTypes.h"

#include "Scope/GeneralPurpose/IIntStream.h"

#include <stddef.h>
#include <stdint.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __IntRingBufferPrivateData* IntRingBufferHandle;

/******************************************************************************
 Public functions
******************************************************************************/
/**
 * Constructor
 * @param capacity
 * @return
 */
IntRingBufferHandle IntRingBuffer_create(size_t capacity);

/**
 * Returns the capacity of the buffer
 * @param self
 * @return
 */
size_t IntRingBuffer_getCapacity(IntRingBufferHandle self);

/**
 * Returns the amount of empty space in the buffer
 * @param self
 * @return
 */
size_t IntRingBuffer_getNumberOfFreeData(IntRingBufferHandle self);

/**
 * Returns the amount of filled space in the buffer
 * @param self
 * @return
 */
size_t IntRingBuffer_getNumberOfUsedData(IntRingBufferHandle self);

/**
 * Writes the data in to the buffer. The amount of written data will be returned.
 * @param self
 * @param data Array from which the data will be copied
 * @param length Amount of data to be written
 * @return If length > ByteRingBuffer_getNumberOfFreeData, -1 will be returned. Otherwise the amount of written
 *         data is returned
 */
int IntRingBuffer_write(IntRingBufferHandle self, const uint32_t* data, size_t length);

/**
 * Reads newest data and writes them in the passed byte array
 * @param self
 * @param data Array into which the data has to be written
 * @param length Max length of the data array.
 * @return If length < ByteRingBuffer_getNumberOfUsedData, -1 will be returned. Otherwise the amount of read data
 *          returned.
 */
int IntRingBuffer_read(IntRingBufferHandle self, uint32_t* data, size_t length);

/**
 * Same as a normal read operation, but resets the tail pointer to its original position.
 * therefore data can be read without being destroyed.
 * @param self
 * @param data
 * @param length
 * @return
 */
int IntRingBuffer_readNoPosInc(IntRingBufferHandle self, uint32_t* data, size_t length);

/**
 * Resets head and tail to their original position and therefore clearing the channel
 * @param self
 */
void IntRingBuffer_clear(IntRingBufferHandle self);

/**
 * Deconstructor
 * @param self
 */
void IntRingBuffer_destroy(IntRingBufferHandle self);

#endif
