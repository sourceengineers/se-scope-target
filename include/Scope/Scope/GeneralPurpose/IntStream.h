/*!*****************************************************************************
 * @file         IntStream.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the IntStream interface The stream currently
 *               works with a simple array internally.
 *               The array will be swapped to a ringbuffer in a later version.
 *               This will enable a more dynamic and fluent usage of the stream.
 ******************************************************************************/

#ifndef INTSTREAM_H_
#define INTSTREAM_H_

#include <Scope/GeneralPurpose/IIntStream.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <Scope/GeneralPurpose/DataTypes.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __IntStreamPrivateData* IntStreamHandle;

/******************************************************************************
 Public functions
******************************************************************************/
/* Constructor: Creates a new instance of the stream */
IntStreamHandle IntStream_create(size_t capacity);

/* Deconstructor: Deletes the instance of the stream */
void IntStream_destroy(IntStreamHandle self);

/* Returns the IIntStream interface */
IIntStreamHandle IntStream_getIIntStream(IntStreamHandle self);

/* Flushes the stream */
void IntStream_flush(IntStreamHandle self);

/* Returns the amount of data currently pending in the stream */
size_t IntStream_length(IntStreamHandle self);

/* Writes "length" amount of data into the stream */
void IntStream_write(IntStreamHandle self, const gemmi_uint* data, const size_t length);

/* Writes one single data point into the stream */
void IntStream_writeData(IntStreamHandle self, const gemmi_uint data);

/* Reads "length" amount of data from the stream and writes it into "data" */
void IntStream_read(IntStreamHandle self, gemmi_uint* data, const size_t length);

/* Returns one single data point */
gemmi_uint IntStream_readData(IntStreamHandle self);

/* Returns of a data point is ready to be read or not */
bool IntStream_dataIsReady(IntStreamHandle self);

#endif
