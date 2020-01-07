/*!****************************************************************************************************************************************
 * @file         JsonPacker.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach <samuel.schuepbach@sourceengineers.com>
 *
 * @brief        Implements the serialisation of the scope data for the json format by implementing the
 *               IPacker interface.
 *
 *****************************************************************************************************************************************/

#ifndef JSONPACKER_H_
#define JSONPACKER_H_

#include <se-lib-c/stream/IByteStream.h>

#include "Scope/GeneralPurpose/DataTypes.h"
#include "Scope/Communication/ICommunicator.h"
#include "Scope/Control/IPacker.h"
#include "Scope/Core/ScopeTypes.h"

#include <stddef.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __JsonPackerPrivateData* JsonPackerHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/**
 * Constructor
 * @param maxNumberOfChannels
 * @param maxAddressesToAnnounce
 * @param byteStream A reference to the output stream
 * @return
 */
JsonPackerHandle JsonPacker_create(size_t maxNumberOfChannels, size_t maxAddressesToAnnounce,
                                   IByteStreamHandle byteStream);

/**
 * Calculates how much space the buffers need
 * This is used, so that the outbut byte buffer can be defined without having to be generated in the JsonPacker
 * @param maxNumberOfChannels
 * @param sizeOfChannels
 * @param maxAddressesToAnnounce
 * @return
 */
size_t JsonPacker_calculateBufferSize(size_t maxNumberOfChannels, size_t sizeOfChannels, size_t maxAddressesToAnnounce);

/**
 * Returns the packer interface
 * @param self
 * @return
 */
IPackerHandle JsonPacker_getIPacker(JsonPackerHandle self);

/**
 * Deconstructor
 * @param self
 */
void JsonPacker_destroy(JsonPackerHandle self);

#endif
