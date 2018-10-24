/*!****************************************************************************************************************************************
 * @file         MsgpackCommon.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach <samuel.schuepbach@sourceengineers.com>
 *
 * @brief        Implements functions for which get used by both the packer and the unpacker
 *
 *****************************************************************************************************************************************/

#ifndef MSGPACKCOMMON_H_
#define MSGPACKCOMMON_H_

#include <msgpack.h>


void Msgpack_printObjFromByte(const uint8_t* data, const size_t length);

void Msgpack_printAsBytes(const uint8_t* data, const size_t length);


#endif
