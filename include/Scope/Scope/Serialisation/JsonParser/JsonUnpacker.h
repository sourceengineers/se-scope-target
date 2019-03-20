/*!****************************************************************************************************************************************
 * @file         JsonUnpacker.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach <samuel.schuepbach@sourceengineers.com>
 *
 * @brief        Implements the deserialisation of input data for the json format by implementing the
 *               IUnpacker interface.
 *               It uses the jsmn library to do so.
 *
 *****************************************************************************************************************************************/

#ifndef JSONUNPACKER_H_
#define JSONUNPACKER_H_

#include <Scope/Control/IUnpacker.h>
#include <Scope/GeneralPurpose/BufferedByteStream.h>
#include <Scope/GeneralPurpose/DataTypes.h>
#include <Scope/GeneralPurpose/IByteStream.h>

#include <stddef.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __JsonUnpackerPrivateData* JsonUnpackerHandle;

/******************************************************************************
 Public functions
******************************************************************************/
/**
 * Constructor
 * @param stream Input stream
 * @return
 */
JsonUnpackerHandle JsonUnpacker_create(IByteStreamHandle stream);

/**
 * Returns the IUnpacker interface
 * @param self
 * @return
 */
IUnpackerHandle JsonUnpacker_getIUnpacker(JsonUnpackerHandle self);

/**
 * Estimates how big the input buffer needs to be
 * @return
 */
size_t JsonUnpacker_calculateBufferSize(void);

/**
 * Deconstructor
 * @param self
 */
void JsonUnpacker_destroy(JsonUnpackerHandle self);

#endif
