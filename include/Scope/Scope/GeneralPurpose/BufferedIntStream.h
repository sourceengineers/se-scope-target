/*!*****************************************************************************
 * @file         BufferedIntStream.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the BufferedIntStream interface The stream currently
 *               works with a simple array internally.
 *               The array will be swapped to a ringbuffer in a later version.
 *               This will enable a more dynamic and fluent usage of the stream.
 ******************************************************************************/

#ifndef INTSTREAM_H_
#define INTSTREAM_H_

#include <Scope/GeneralPurpose/DataTypes.h>
#include <Scope/GeneralPurpose/IIntStream.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __BufferedIntStreamPrivateData* BufferedIntStreamHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the stream */
BufferedIntStreamHandle BufferedIntStream_create(size_t capacity);

/* Deconstructor: Deletes the instance of the stream */
void BufferedIntStream_destroy(BufferedIntStreamHandle self);

/* Returns the IIntStream interface */
IIntStreamHandle BufferedIntStream_getIIntStream(BufferedIntStreamHandle self);

/* Flushes the stream */
void BufferedIntStream_flush(BufferedIntStreamHandle self);

/* Returns the amount of data currently pending in the stream */
size_t BufferedIntStream_length(BufferedIntStreamHandle self);

/* Writes "length" amount of data into the stream */
void BufferedIntStream_write(BufferedIntStreamHandle self, const uint32_t* data, const size_t length);

/* Writes one single data point into the stream */
void BufferedIntStream_writeInt(BufferedIntStreamHandle self, const uint32_t data);

/* Reads "length" amount of data from the stream and writes it into "data" */
void IntStream_read(BufferedIntStreamHandle self, uint32_t* data, const size_t length);

/* Returns one single data point */
uint32_t IntStream_readInt(BufferedIntStreamHandle self);

/* Returns of a data point is ready to be read or not */
bool IntStream_byteIsReady(BufferedIntStreamHandle self);

#endif
