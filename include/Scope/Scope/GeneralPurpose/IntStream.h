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
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>

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
IIntStreamHandle IntStream_getIntStream(IntStreamHandle self);


#endif
