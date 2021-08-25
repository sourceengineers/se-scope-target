/*!*****************************************************************************
 * @file         FloatRingBuffer.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of a ringbuffer for float values.
 *
 ******************************************************************************/

#ifndef FLOATRINGBUFFER_H_
#define FLOATRINGBUFFER_H_

#include <se-lib-c/stream/IFloatStream.h>
#include <stddef.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __FloatRingBufferPrivateData* FloatRingBufferHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/**
 * Constructor
 * @param capacity
 * @return
 */
FloatRingBufferHandle FloatRingBuffer_create(size_t capacity);

/**
 * Returns the capacity of the buffer
 * @param self
 * @return
 */
size_t FloatRingBuffer_getCapacity(FloatRingBufferHandle self);

/**
 * Returns the amount of empty space in the buffer
 * @param self
 * @return
 */
size_t FloatRingBuffer_getNumberOfFreeData(FloatRingBufferHandle self);

/**
 * Returns the amount of filled space in the buffer
 * @param self
 * @return
 */size_t FloatRingBuffer_getNumberOfUsedData(FloatRingBufferHandle self);

/**
 * Writes the data in to the buffer. The amount of written data will be returned.
 * @param self
 * @param data Array from which the data will be copied
 * @param length Amount of data to be written
 * @return If length > ByteRingBuffer_getNumberOfFreeData, -1 will be returned. Otherwise the amount of written
 *         data is returned
 */
int FloatRingBuffer_write(FloatRingBufferHandle self, const float* data, const size_t length);

/**
 * Reads newest data and writes them in the passed byte array
 * @param self
 * @param data Array into which the data has to be written
 * @param length Max length of the data array.
 * @return If length < ByteRingBuffer_getNumberOfUsedData, -1 will be returned. Otherwise the amount of read data
 *          returned.
 */
int FloatRingBuffer_read(FloatRingBufferHandle self, float* data, const size_t length);

/**
 * Same as a normal read operation, but resets the tail pointer to its original position.
 * therefore data can be read without being destroyed.
 * @param self
 * @param data
 * @param length
 * @return
 */
int FloatRingBuffer_readNoPosInc(FloatRingBufferHandle self, float* data, const size_t length);

/**
 * Resets head and tail to their original position and therefore clearing the channel
 * @param self
 */
void FloatRingBuffer_clear(FloatRingBufferHandle self);

/**
 * Deconstructor
 * @param self
 */
void FloatRingBuffer_destroy(FloatRingBufferHandle self);


#endif
