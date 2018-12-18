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

#ifndef JSONPACKER_H_
#define JSONPACKER_H_

#include <jsmn.h>
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
JsonUnpackerHandle JsonUnpacker_create(const size_t msgLength);

/* Destroys the instance of the json unpacker */
void JsonUnpacker_destroy(JsonUnpackerHandle self);

/* Returns the IUnpacker interface */
IUnpackerHandle JsonUnpacker_getIUnpacker(JsonUnpackerHandle self);
#endif
