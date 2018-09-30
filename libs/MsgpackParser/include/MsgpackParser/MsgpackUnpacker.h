/*!****************************************************************************************************************************************
 * @file         msgpack_parser.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach <samuel.schuepbach@sourceengineers.com>
 *
 * @brief        Unpacks incoming msgpack data
 *
 *****************************************************************************************************************************************/

#ifndef MSGPACKUNPACKER_H_
#define MSGPACKUNPACKER_H_

#include <msgpack.h>
#include <Communication/IUnpacker.h>


/* Defines class */
typedef struct __MsgpackUnpackerPrivateData* MsgpackUnpackerHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instanze of the factory */
MsgpackUnpackerHandle MsgpackUnpacker_create(const size_t msgLength);

/* Destroys the instance of the msgpack unpacker */
void MsgpackUnpacker_destroy(MsgpackUnpackerHandle self);

/* Returns the Iparser interface */
IUnpackerHandle MsgpackUnpacker_getIUnpacker(MsgpackUnpackerHandle self);
#endif
