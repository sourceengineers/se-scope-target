/*!****************************************************************************************************************************************
 * @file         JsonUnpacker.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach <samuel.schuepbach@sourceengineers.com>
 *
 * @brief        Implements functions of the IUnpacker.h interface.
 *
 *****************************************************************************************************************************************/

#ifndef JSONUNPACKER_H_
#define JSONUNPACKER_H_

#include <jsmn/jsmn.h>
#include <Scope/Control/IUnpacker.h>
#include <Scope/GeneralPurpose/DataTypes.h>
#include <Scope/GeneralPurpose/BufferedByteStream.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __JsonUnpackerPrivateData* JsonUnpackerHandle;

/******************************************************************************
 Public functions
******************************************************************************/
/* Constructor: Creates a new instance of the json unpacker */
JsonUnpackerHandle JsonUnpacker_create(IByteStreamHandle stream);

/* Destroys the instance of the json unpacker */
void JsonUnpacker_destroy(JsonUnpackerHandle self);

/* Returns the IUnpacker interface */
IUnpackerHandle JsonUnpacker_getIUnpacker(JsonUnpackerHandle self);

/* Calculates how big the buffer has to be */
size_t JsonUnpacker_calculateBufferSize();

#endif
