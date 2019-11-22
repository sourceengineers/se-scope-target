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

#include "Scope/GeneralPurpose/DataTypes.h"
#include "Scope/GeneralPurpose/IByteStream.h"

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
 * Deconstructor
 * @param self
 */
void BufferedByteStream_destroy(BufferedByteStreamHandle self);

#endif
