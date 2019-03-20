/*!*****************************************************************************
 * @file         BufferedByteStream.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the byte stream interface.
 *               The buffered byte stream uses a ring buffer as underlying data
 *               structure
 *
 ******************************************************************************/

#ifndef BYTESTREAM_H_
#define BYTESTREAM_H_

#include <Scope/GeneralPurpose/DataTypes.h>
#include <Scope/GeneralPurpose/IByteStream.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __BufferedByteStreamPrivateData* BufferedByteStreamHandle;

/******************************************************************************
 Public functions
******************************************************************************/
/**
 * Constructor
 * @param capacity size of the buffer
 * @return
 */
BufferedByteStreamHandle BufferedByteStream_create(size_t capacity);

/**
 * Returns the stream interface
 * @param self
 * @return
 */
IByteStreamHandle BufferedByteStream_getIByteStream(BufferedByteStreamHandle self);

/**
 * Flushes the stream
 * @param self
 */
void BufferedByteStream_flush(BufferedByteStreamHandle self);

/**
 * Returns the amount of data currently pending in the stream
 * @param self
 * @return
 */
size_t BufferedByteStream_length(BufferedByteStreamHandle self);

/**
 * Writes "length" amount of data into the stream
 * @param self
 * @param data Data from which the data has to be written
 * @param length Amount of data that has to be written
 */
void BufferedByteStream_write(BufferedByteStreamHandle self, const uint8_t* data, size_t length);

/**
 * Writes one single data point into the stream
 * @param self
 * @param data
 */
void BufferedByteStream_writeByte(BufferedByteStreamHandle self, uint8_t data);

/**
 * Reads length amount of data from the stream and writes it into data
 * @param self
 * @param data Array into which the data gets written
 * @param length Size of array. If this is smaller than the amount of data pending, the function gets rejected.
 */
void ByteStream_read(BufferedByteStreamHandle self, uint8_t* data, size_t length);

/**
 * Returns one single data point
 * @param self
 * @return
 */
uint8_t ByteStream_readByte(BufferedByteStreamHandle self);

/**
 * Returns of more data is pending or not
 * @param self
 * @return True if data pending. False if not
 */
bool ByteStream_byteIsReady(BufferedByteStreamHandle self);

/**
 * Deconstructor
 * @param self
 */
void BufferedByteStream_destroy(BufferedByteStreamHandle self);

#endif
