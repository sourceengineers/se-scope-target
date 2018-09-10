/*!****************************************************************************************************************************************
 * @file         msgpack_parser.h
 *
 * @copyright    Copyright (c) 2018 by Sonnen. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach <samuel.schuepbach@sourceengineers.com>
 *
 * @brief        Unpacks incoming msgpack data
 *
 *****************************************************************************************************************************************/

#ifndef msgpack_parser_H_
#define msgpack_parser_H_

#include <msgpack_parser/msgpack_base_unpacker.h>


typedef struct Msgpack_unpacker_struct
{
  /* Attributes*/
  msgpack_unpacked und;   /* unpacked data */
  msgpack_unpack_return ret; /* unpacking return data */
  msgpack_object obj;
  msgpack_unpacker unp;

  Msgpack_base_unpacker* super;

  /* Methodes */
  void(*unpack)(struct Msgpack_unpacker_struct* self, char* data, int length);
  void(*printMsgpackObj)(struct Msgpack_unpacker_struct* self, msgpack_object obj);
  msgpack_object(*getCmdObj)(struct Msgpack_unpacker_struct* self, char* key);


  /* Parent methodes */
  void(*printObj)(msgpack_object obj);
  msgpack_object(*matchObjToKey)(msgpack_object parentObj, char *key);
  void(*cpyStr)(char *str, char *data, int size);
  msgpack_object(*matchKeyToObj)(msgpack_object parentObj, char *key);
  int(*matchKeyToIndex)(msgpack_object parentObj, char *key);
  int(*getIntFromObj)(msgpack_object obj, int offset);
  float(*getFloatFromObj)(msgpack_object obj, int offset);
  bool(*getBoolFromObj)(msgpack_object obj, int offset);
  void(*getStringFromObj)(msgpack_object obj, int offset, char *targetStr, int maxLenght);

} Msgpack_unpacker ;


Msgpack_unpacker* msgpack_unpacker_create();
void msgpack_unpacker_unpack(Msgpack_unpacker* self, char *data, int request_size);
msgpack_object Msgpack_unpacker_getCmdObj(Msgpack_unpacker* self, char *key);

#endif // msgpack_parser_H_
