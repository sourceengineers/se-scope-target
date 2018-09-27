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
#include <Communication/IParser.h>


/* Defines class */
typedef struct __MsgpackUnpackerPrivateData* MsgpackUnpackerHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instanze of the factory */
MsgpackUnpackerHandle MsgpackUnpacker_create(const size_t msgLength);

void MsgpackUnpacker_destroy(MsgpackUnpackerHandle self);

/* Returns the Iparser interface */
IParserHandle MsgpackUnpacker_getIParser(MsgpackUnpackerHandle self);
#endif
