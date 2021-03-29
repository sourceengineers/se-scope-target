/*!****************************************************************************************************************************************
 * @file         NanopbUnpacker.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @license      This file is part of SE-Scope.
 *
 *               SE-Scope is free software; you can redistribute it and/or
 *               modify it under the terms of the GNU General Public License as
 *               published by the Free Software Foundation.
 *
 *               SE-Scope is distributed in the hope that it will be useful,
 *               but WITHOUT ANY WARRANTY; without even the implied warranty of
 *               MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *               GNU General Public License for more details.
 *
 *               You should have received a copy of the GNU General Public License
 *               along with SE-Scope.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @authors      Samuel Schuepbach <samuel.schuepbach@sourceengineers.com>
 *
 * @brief        Implements the serialisation of the scope data for the protobuf format by implementing the
 *               IUnpacker interface.
 *
 *****************************************************************************************************************************************/

#ifndef NANOPBUNPACKER_H_
#define NANOPBUNPACKER_H_

#include <se-lib-c/stream/IByteStream.h>

#include "Scope/GeneralPurpose/DataTypes.h"
#include "Scope/Communication/ICommunicator.h"
#include "Scope/Control/IUnpacker.h"
#include "Scope/Core/ScopeTypes.h"

#include <stddef.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __NanopbUnpackerPrivateData* NanopbUnpackerHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/**
 * Constructor
 * @param input
 * @param amountOfChannels
 * @return
 */
NanopbUnpackerHandle NanopbUnpacker_create(IByteStreamHandle input, size_t amountOfChannels);

/**
 * Calculates how much space the buffers need
 * This is used, so that the input byte buffer can be defined without having to be generated in the NanopbUnpacker
 * @param maxNumberOfChannels
 * @return
 */
size_t NanopbUnpacker_calculateBufferSize(size_t maxNumberOfChannels);

/**
 * Returns the packer interface
 * @param self
 * @return
 */
IUnpackerHandle NanopbUnpacker_getIUnpacker(NanopbUnpackerHandle self);

/**
 * Deconstructor
 * @param self
 */
void NanopbUnpacker_destroy(NanopbUnpackerHandle self);

#endif
