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
#include <Scope/Communication/IUnpacker.h>
#include <Scope/GeneralPurpose/DataTypes.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __JsonUnpackerPrivateData* JsonUnpackerHandle;

/******************************************************************************
 Public functions
******************************************************************************/
/* Constructor: Creates a new instance of the json unpacker */
JsonUnpackerHandle JsonUnpacker_create();

/* Destroys the instance of the json unpacker */
void JsonUnpacker_destroy(JsonUnpackerHandle self);

/* Returns the IUnpacker interface */
IUnpackerHandle JsonUnpacker_getIUnpacker(JsonUnpackerHandle self);

size_t JsonUnpacker_calculateBufferSize();

#endif
