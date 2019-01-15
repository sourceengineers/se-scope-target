/*!*****************************************************************************
 * @file         BufferedByteStream.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the BufferedByteStream interface The stream currently
 *               works with a simple array internally.
 *               The array will be swapped to a ringbuffer in a later version.
 *               This will enable a more dynamic and fluent usage of the stream.
 ******************************************************************************/
 
#ifndef BYTESTREAM_H_
#define BYTESTREAM_H_

#include <Scope/GeneralPurpose/IByteStream.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <Scope/GeneralPurpose/IByteStream.h>
#include <Scope/GeneralPurpose/DataTypes.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __BufferedByteStreamPrivateData* BufferedByteStreamHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the stream */
BufferedByteStreamHandle BufferedByteStream_create(size_t capacity);

/* Deconstructor: Deletes the instance of the stream */
void BufferedByteStream_destroy(BufferedByteStreamHandle self);

/* Returns the IByteStream interface */
IByteStreamHandle BufferedByteStream_getIByteStream(BufferedByteStreamHandle self);

/* Flushes the stream */
void BufferedByteStream_flush(BufferedByteStreamHandle self);

/* Returns the amount of data currently pending in the stream */
size_t BufferedByteStream_length(BufferedByteStreamHandle self);

/* Writes "length" amount of data into the stream */
void BufferedByteStream_write(BufferedByteStreamHandle self, const uint8_t* data, const size_t length);

/* Writes one single data point into the stream */
void BufferedByteStream_writeByte(BufferedByteStreamHandle self, const uint8_t data);

/* Reads "length" amount of data from the stream and writes it into "data" */
void ByteStream_read(BufferedByteStreamHandle self, uint8_t* data, const size_t length);

/* Returns one single data point */
uint8_t ByteStream_readByte(BufferedByteStreamHandle self);

/* Returns of a data point is ready to be read or not */
bool ByteStream_byteIsReady(BufferedByteStreamHandle self);

#endif
