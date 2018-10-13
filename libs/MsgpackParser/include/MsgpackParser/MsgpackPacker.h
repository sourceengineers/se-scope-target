/*!****************************************************************************************************************************************
 * @file         MsgpackPacker.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach <samuel.schuepbach@sourceengineers.com>
 *
 * @brief        Implements functions of the IPacker.h interface.
 *
 *****************************************************************************************************************************************/

#ifndef MSGPACKPACKER_H_
#define MSGPACKPACKER_H_

#include <msgpack.h>
#include <Communication/IPacker.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __MsgpackPackerPrivateData* MsgpackPackerHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the msgpack Packer */
MsgpackPackerHandle MsgpackPacker_create(const size_t msgLength, const size_t maxNumberOfChannels,
                                         IByteStreamHandle byteStream);

/* Destroys the instance of the msgpack Packer */
void MsgpackPacker_destroy(MsgpackPackerHandle self);

/* Returns the IPacker interface */
IPackerHandle MsgpackPacker_getIPacker(MsgpackPackerHandle self);
#endif
