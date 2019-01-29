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

#include <Scope/Control/IPacker.h>
#include <Scope/Communication/ICommunicator.h>
#include <Scope/Core/AddressStorage.h>
#include <Scope/GeneralPurpose/DataTypes.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __JsonPackerPrivateData* JsonPackerHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the Packer */
JsonPackerHandle JsonPacker_create(size_t maxNumberOfChannels, size_t maxAddressesToAnnounce,
                                   IByteStreamHandle byteStream);

/* Destroys the instance of the Packer */
void JsonPacker_destroy(JsonPackerHandle self);

/* Calculates how much space the buffers need
 * This is used, so that the outbut byte buffer can be defined without having to be generated in the JsonPacker*/
size_t JsonPacker_calculateBufferSize(size_t maxNumberOfChannels, size_t sizeOfChannels, size_t maxAddressesToAnnounce);

/* Returns the IPacker interface */
IPackerHandle JsonPacker_getIPacker(JsonPackerHandle self);
#endif
