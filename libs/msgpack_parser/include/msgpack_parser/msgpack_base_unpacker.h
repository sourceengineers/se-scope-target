/*!****************************************************************************************************************************************
 * @file         msgpack_base_unpacker.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach <samuel.schuepbach@sourceengineers.com>
 *
 * @brief        Impelments functions to help comand parsers and upacker to parse the msgpack data easily.
 *
 *****************************************************************************************************************************************/

#ifndef msgpack_base_unpacker_H_
#define msgpack_base_unpacker_H_

#include <msgpack.h>

typedef struct Msgpack_base_unpacker_struct
{
  void(*printObj)(msgpack_object obj);
  void(*cpyStr)(char *str, char *data, int size);
  msgpack_object(*matchKeyToObj)(msgpack_object parentObj, char *key);
  int(*matchKeyToIndex)(msgpack_object parentObj, char *key);
  int(*getIntFromObj)(msgpack_object obj, int offset);
  float(*getFloatFromObj)(msgpack_object obj, int offset);
  bool(*getBoolFromObj)(msgpack_object obj, int offset);
  void(*getStringFromObj)(msgpack_object obj, int offset, char *targetStr, int maxLenght);

} Msgpack_base_unpacker ;


/* Helper Functions */
Msgpack_base_unpacker* msgpack_base_unpacker_create();
msgpack_object msgpack_base_unpacker_match_key_to_obj(msgpack_object parentObj, char *key);
void msgpack_unpacker_copy_str(char *str, char *data, int size);
void msgpack_base_unpacker_print_obj(msgpack_object obj);
msgpack_object msgpack_base_unpacker_match_key_to_obj(msgpack_object parentObj, char *key);
int msgpack_base_unpacker_match_key_to_index(msgpack_object parentObj, char *key);

/* Data fetcher functions */
int msgpack_base_unpacker_get_int_from_obj(msgpack_object obj, int offset);
float msgpack_base_unpacker_get_float_from_obj(msgpack_object obj, int offset);
bool msgpack_base_unpacker_get_bool_from_obj(msgpack_object obj, int offset);
void msgpack_base_unpacker_get_string_from_obj(msgpack_object obj, int offset, char *targetStr, int maxLenght);

#endif // msgpack_parser_H_
