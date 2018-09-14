/*!****************************************************************************************************************************************
 * @file         ring_buffer.h
 *
 * @copyright    Copyright (c) 2018 by Sonnen. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samue.schuepbach@sourceengineers.com
 *
 * @brief        TODO
 *
 *****************************************************************************************************************************************/

#include <scope/i_float_stream.h>

/* Defines Class name */
typedef struct RingBufferStruct RingBuffer;


/* Constructor: Creates a new instanze of the buffer */
RingBuffer* RingBuffer_create(size_t capacity);

/* Returns the capacity of the buffer */
size_t RingBuffer_getCapacity(RingBuffer* self);

/* Returns the amount of free data points in the buffer */
size_t RingBuffer_freeData(RingBuffer* self);

/* Returns the amout of used data points in the buffer */
size_t RingBuffer_usedData(RingBuffer* self);

/* Writes the data in to the buffer. The amount of written data will be returned.
   If the write operation would let the buffer overflow, a -1 will be returned.*/
ssize_t RingBuffer_writeData(RingBuffer* self, float* data, size_t length);

/* Reads newest data and writes them in the passed foat array
   The amount of safed data points will be returned.
   if the length is smaller than the amount of data to be read, the function
   will return -1 */
ssize_t RingBuffer_readData(RingBuffer* self, float* data, size_t length);

/* Returns the amout of data ready to be read */
size_t RingBuffer_IFloatStream_getSize(IFloatStream *self);

/* Returns one data point with each call */
float RingBuffer_IFloatStream_getData(IFloatStream *self);

/* Returns the IFloatStream interface */
IFloatStream* RingBuffer_getIFloatStream(IFloatStream *self);
