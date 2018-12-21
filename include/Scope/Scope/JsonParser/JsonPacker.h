/*!****************************************************************************************************************************************
 * @file         JsonPacker.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach <samuel.schuepbach@sourceengineers.com>
 *
 * @brief        Implements functions of the IPacker.h for the Json interface.
 *
 *****************************************************************************************************************************************/

#ifndef JSONPACKER_H_
#define JSONPACKER_H_

#include <Scope/Communication/IPacker.h>
#include <Scope/Communication/IComValidator.h>
#include <Scope/AddressStorage.h>
#include <Scope/GeneralPurpose/DataTypes.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __JsonPackerPrivateData* JsonPackerHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the msgpack Packer */
JsonPackerHandle JsonPacker_create(size_t maxNumberOfChannels, size_t maxAddressesToAnnounce,
                                   IComValidatorHandle validator,
                                   IByteStreamHandle byteStream);

/* Destroys the instance of the msgpack Packer */
void JsonPacker_destroy(JsonPackerHandle self);

/* Calculates how much space the buffers need
 * This is used, so that the outbut byte buffer can be defined without having to be generated in the JsonPacker*/
size_t JsonPacker_calculateBufferSizes(size_t maxNumberOfChannels, size_t maxAddressesToAnnounce, size_t sizeOfChannels);

/* Returns the IPacker interface */
IPackerHandle JsonPacker_getIPacker(JsonPackerHandle self);
#endif
