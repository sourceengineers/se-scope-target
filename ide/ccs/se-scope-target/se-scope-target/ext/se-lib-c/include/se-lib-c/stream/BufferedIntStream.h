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

#include <se-lib-c/definition/SeLibCTypes.h>
#include <se-lib-c/stream/IIntStream.h>
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
 * Deconstructor
 * @param self
 */
void BufferedIntStream_destroy(BufferedIntStreamHandle self);

#endif
