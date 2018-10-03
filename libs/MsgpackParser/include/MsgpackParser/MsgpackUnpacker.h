/*!****************************************************************************************************************************************
 * @file         MsgpackUnpacker.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach <samuel.schuepbach@sourceengineers.com>
 *
 * @brief        Implements functions of the IUnpacker.h interface.
 *
 *****************************************************************************************************************************************/

#ifndef MSGPACKUNPACKER_H_
#define MSGPACKUNPACKER_H_

#include <msgpack.h>
#include <Communication/IUnpacker.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __MsgpackUnpackerPrivateData* MsgpackUnpackerHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instanze of the msgpack unpacker */
MsgpackUnpackerHandle MsgpackUnpacker_create(const size_t msgLength);

/* Destroys the instance of the msgpack unpacker */
void MsgpackUnpacker_destroy(MsgpackUnpackerHandle self);

/* Returns the IUnpacker interface */
IUnpackerHandle MsgpackUnpacker_getIUnpacker(MsgpackUnpackerHandle self);
#endif
