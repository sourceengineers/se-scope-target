/*!*****************************************************************************
 * @file         BufferedIntStream.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the int stream interface.
 *               The buffered int stream uses a ring buffer as underlying data
 *               structure
 *
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
/**
 * Constructor
 * @param capacity size of the buffer
 * @return
 */
BufferedIntStreamHandle BufferedIntStream_create(size_t capacity);

/**
 * Returns the stream interface
 * @param self
 * @return
 */
IIntStreamHandle BufferedIntStream_getIIntStream(BufferedIntStreamHandle self);

/**
 * Flushes the stream
 * @param self
 */
void BufferedIntStream_flush(BufferedIntStreamHandle self);

/**
 * Returns the amount of data currently pending in the stream
 * @param self
 * @return
 */
size_t BufferedIntStream_length(BufferedIntStreamHandle self);

/**
 * Writes "length" amount of data into the stream
 * @param self
 * @param data Data from which the data has to be written
 * @param length Amount of data that has to be written
 */
void BufferedIntStream_write(BufferedIntStreamHandle self, const uint32_t* data, const size_t length);

/**
 * Writes one single data point into the stream
 * @param self
 * @param data
 */
void BufferedIntStream_writeInt(BufferedIntStreamHandle self, const uint32_t data);

/**
 * Reads length amount of data from the stream and writes it into data
 * @param self
 * @param data Array into which the data gets written
 * @param length Size of array. If this is smaller than the amount of data pending, the function gets rejected.
 */
void IntStream_read(BufferedIntStreamHandle self, uint32_t* data, const size_t length);

/**
 * Returns one single data point
 * @param self
 * @return
 */
uint32_t IntStream_readInt(BufferedIntStreamHandle self);

/**
 * Returns of more data is pending or not
 * @param self
 * @return True if data pending. False if not
 */
bool IntStream_byteIsReady(BufferedIntStreamHandle self);

/**
 * Deconstructor
 * @param self
 */
void BufferedIntStream_destroy(BufferedIntStreamHandle self);

#endif
