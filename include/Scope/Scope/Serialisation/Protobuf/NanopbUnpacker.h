/*!****************************************************************************************************************************************
 * @file         NanopbUnpacker.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach <samuel.schuepbach@sourceengineers.com>
 *
 * @brief        Implements the serialisation of the scope data for the protobuf format by implementing the
 *               IUnpacker interface.
 *
 *****************************************************************************************************************************************/

#ifndef NANOPBUNPACKER_H_
#define NANOPBUNPACKER_H_

#include "../../../../../ext/se-lib-c/public/se-lib-c/stream/IByteStream.h"

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
