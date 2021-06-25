/*!****************************************************************************************************************************************
 * @file         NanopbPacker.h
 *
 * @copyright    Copyright (c) 2021 by Source Engineers GmbH. All Rights Reserved.
 *
 * @license {    This file is part of se-scope-target.
 *
 *               se-scope-target is free software; you can redistribute it and/or
 *               modify it under the terms of the GPLv3 General Public License Version 3
 *               as published by the Free Software Foundation.
 *
 *               se-scope-target is distributed in the hope that it will be useful,
 *               but WITHOUT ANY WARRANTY; without even the implied warranty of
 *               MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *               GNU General Public License for more details.
 *
 *               You should have received a copy of the GPLv3 General Public License Version 3
 *               along with se-scope-target.  If not, see <http://www.gnu.org/licenses/>.
 *
 *               In closed source or commercial projects, GPLv3 General Public License Version 3
 *               is not valid. In this case the commercial license received with the purchase
 *               is applied (See SeScopeLicense.pdf).
 *               Please contact us at scope@sourceengineers.com for a commercial license.
 * }
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
#include <se-lib-c/stream/IByteStream.h>

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
