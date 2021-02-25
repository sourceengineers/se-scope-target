/*!****************************************************************************************************************************************
 * @file         NanopbPacker.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach <samuel.schuepbach@sourceengineers.com>
 *
 * @brief        Implements the serialisation of the scope data for the protobuf format by implementing the
 *               IPacker interface.
 *
 *****************************************************************************************************************************************/

#ifndef NANOPBPACKER_H_
#define NANOPBPACKER_H_

#include "Scope/GeneralPurpose/DataTypes.h"
#include "../../../../../ext/se-lib-c/public/se-lib-c/stream/IByteStream.h"

#include "Scope/Communication/ICommunicator.h"
#include "Scope/Control/IPacker.h"
#include "Scope/Core/ScopeTypes.h"

#include <stddef.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __NanopbPackerPrivateData* NanopbPackerHandle;

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
NanopbPackerHandle NanopbPacker_create(size_t maxNumberOfChannels, size_t maxAddressesToAnnounce,
                                   IByteStreamHandle byteStream);

/**
 * Calculates how much space the buffers need
 * This is used, so that the outbut byte buffer can be defined without having to be generated in the NanopbPacker
 * @param maxNumberOfChannels
 * @param sizeOfChannels
 * @param maxAddressesToAnnounce
 * @return
 */
size_t NanopbPacker_calculateBufferSize(size_t maxNumberOfChannels, size_t sizeOfChannels, size_t maxAddressesToAnnounce);

/**
 * Returns the packer interface
 * @param self
 * @return
 */
IPackerHandle NanopbPacker_getIPacker(NanopbPackerHandle self);

/**
 * Deconstructor
 * @param self
 */
void NanopbPacker_destroy(NanopbPackerHandle self);

#endif
