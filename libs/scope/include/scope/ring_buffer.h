/*!*****************************************************************************
 * @file         ring_buffer.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samue.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of a ring buffer. Data can eithet be fetched
 *                through the read function, or through the implementation of
 *                stream interface.
 ******************************************************************************/
 
#ifndef RING_BUFFER_H_
#define RING_BUFFER_H_

#include <scope/i_float_stream.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>

/* Defines class */
/* Define private data */
typedef struct RingBufferPrivateStruct RingBufferPrivate;
/* Define public data */
typedef struct RingBufferStruct RingBuffer;

struct RingBufferStruct
{
  RingBufferPrivate* _private;
  
  /* Public functions */
  size_t (*getCapacity)(RingBuffer* self);
  size_t (*freeData)(RingBuffer* self);
  size_t (*usedData)(RingBuffer* self);
  ssize_t (*write)(RingBuffer* self, const float* data, const size_t length);
  ssize_t (*read)(RingBuffer* self, float* data, const size_t length);
  void (*clear)(RingBuffer* self);
  IFloatStream* (*getFloatStream)(RingBuffer* self);

  /* Interfaces */
  IFloatStream iFloatStream;
};

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instanze of the buffer */
RingBuffer* RingBuffer_create(size_t capacity);

/* Deconstructor: Deletes the instanze of the buffer */
void RingBuffer_destroy(RingBuffer* self);

/* Empties the buffer 
   The capacity of the buffer will not change through this, but the free data
   will increase to be the same as the capacity */
static void RingBuffer_clear(RingBuffer* self);

/* Returns the capacity of the buffer */
static size_t RingBuffer_getCapacity(RingBuffer* self);

/* Returns the amount of free data points in the buffer */
static size_t RingBuffer_freeData(RingBuffer* self);

/* Returns the amout of used data points in the buffer */
static size_t RingBuffer_usedData(RingBuffer* self);

/* Writes the data in to the buffer. The amount of written data will be returned.
   If the write operation would let the buffer overflow, a -1 will be returned.*/
static ssize_t RingBuffer_write(RingBuffer* self, const float* data, const size_t length);

/* Reads newest data and writes them in the passed foat array
   The amount of safed data points will be returned.
   if the length is smaller than the amount of data to be read, the function
   will return -1 */
static ssize_t RingBuffer_read(RingBuffer* self, float* data, const size_t length);

/* Returns the amout of data ready to be read */
static size_t RingBuffer_IFloatStream_getSize(IFloatStream* iFloatStream);

/* Opens the stream to given float array */
static void RingBuffer_IFloatStream_open(IFloatStream* iFloatStream, float* floatStream);

/* Closes the stream on given array */
static void RingBuffer_IFloatStream_close(IFloatStream* iFloatStream);

/* Writes the data into the float array
   Return -1 if the float isn't initialized */
static size_t RingBuffer_IFloatStream_getStream(IFloatStream* iFloatStream);

/* Returns the IFloatStream interface */
static IFloatStream* RingBuffer_getIFloatStream(RingBuffer* self);


/******************************************************************************
 Private Functions 
******************************************************************************/
/* Increments the given index by one */
static float* RingBuffer_nextIndex(RingBuffer* self, float* index);

/* Increments the Head index by one 
   Returns true if the increment was successful
   Returns false if a overflow was detectet */
static bool RingBuffer_incHead(RingBuffer* self);

/* Increments the Tail index by one 
   Returns true if the increment was successful
   Returns false if a overflow was detectet */
static bool RingBuffer_incTail(RingBuffer* self);


#endif
