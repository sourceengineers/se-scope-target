/*!*****************************************************************************
 * @file         BufferedFloatStream.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the float stream interface.
 *               The buffered float stream uses a ring buffer as underlying data
 *               structure
 *
 ******************************************************************************/

#ifndef FLOATSTREAM_H_
#define FLOATSTREAM_H_

#include <Scope/GeneralPurpose/DataTypes.h>
#include <Scope/GeneralPurpose/IFloatStream.h>

#include <stdbool.h>
#include <stddef.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __BufferedFloatStreamPrivateData* BufferedFloatStreamHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/**
 * Constructor
 * @param capacity size of the buffer
 * @return
 */
BufferedFloatStreamHandle BufferedFloatStream_create(size_t capacity);

/**
 * Returns the stream interface
 * @param self
 * @return
 */
IFloatStreamHandle BufferedFloatStream_getIFloatStream(BufferedFloatStreamHandle self);

/**
 * Flushes the stream
 * @param self
 */
void BufferedFloatStream_flush(BufferedFloatStreamHandle self);

/**
 * Returns the amount of data currently pending in the stream
 * @param self
 * @return
 */
size_t BufferedFloatStream_length(BufferedFloatStreamHandle self);

/**
 * Writes "length" amount of data into the stream
 * @param self
 * @param data Data from which the data has to be written
 * @param length Amount of data that has to be written
 */
void BufferedFloatStream_write(BufferedFloatStreamHandle self, const float* data, const size_t length);

/**
 * Writes one single data point into the stream
 * @param self
 * @param data
 */
void BufferedFloatStream_writeFloat(BufferedFloatStreamHandle self, const float data);

/**
 * Reads length amount of data from the stream and writes it into data
 * @param self
 * @param data Array into which the data gets written
 * @param length Size of array. If this is smaller than the amount of data pending, the function gets rejected.
 */
void FloatStream_read(BufferedFloatStreamHandle self, float* data, const size_t length);

/**
 * Returns one single data point
 * @param self
 * @return
 */
float FloatStream_readFloat(BufferedFloatStreamHandle self);

/**
 * Returns of more data is pending or not
 * @param self
 * @return True if data pending. False if not
 */
bool FloatStream_byteIsReady(BufferedFloatStreamHandle self);

/**
 * Deconstructor
 * @param self
 */
void BufferedFloatStream_destroy(BufferedFloatStreamHandle self);

#endif
