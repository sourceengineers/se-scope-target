/*!****************************************************************************************************************************************
 * @file         MsgpackCommon.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <MsgpackParser/MsgpackCommon.h>
#include <msgpack/object.h>
#include <msgpack.h>
#include <string.h>

void Msgpack_printObjFromByte(const uint8_t* data, const size_t length){

  msgpack_zone mempool;
  msgpack_object deserialized;

  /* deserialize the buffer into msgpack_object instance. */
  /* deserialized object is valid during the msgpack_zone instance alive. */
  msgpack_zone_init(&mempool, 2048);

  msgpack_unpack((const char*) data, length, NULL, &mempool, &deserialized);

  /* print the deserialized object. */
  msgpack_object_print(stdout, deserialized);
}