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
#include <Scope/GeneralPurpose/DataTypes.h>


void Msgpack_printObjFromByte(FILE *file, const uint8_t* data, const size_t length);

void Msgpack_printAsBytes(FILE *file, const uint8_t* data, const size_t length);


#endif
