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

#include <se-lib-c/definition/SeLibCTypes.h>
#include <se-lib-c/stream/IFloatStream.h>
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
 * Deconstructor
 * @param self
 */
void BufferedFloatStream_destroy(BufferedFloatStreamHandle self);

#endif
