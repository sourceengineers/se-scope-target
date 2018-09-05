/*!****************************************************************************************************************************************
 * @file         msgpack_parser.h
 *
 * @copyright    Copyright (c) 2018 by Sonnen. All Rights Reserved.
 *
 * @authors      Author <Email>
 *
 * @brief        TODO
 *
 *****************************************************************************************************************************************/

#ifndef msgpack_parser_H_
#define msgpack_parser_H_

#include <msgpack_parser/msgpack_data.h>


typedef struct Msgpack_base_unpacker_struct
{
  void(*printObj)(struct Msgpack_base_unpacker_struct* self, msgpack_object obj);
  
  msgpack_object(*matchObjToKey)(struct Msgpack_base_unpacker_struct* self, msgpack_object parentObj, char *key);
  void(*cpyStr)(struct Msgpack_base_unpacker_struct* self, char *str, char *data, int size);
  void(*printObj)(struct Msgpack_base_unpacker_struct* self, msgpack_object obj);
  msgpack_object(*matchKeyToObj)(struct Msgpack_base_unpacker_struct* self, msgpack_object parentObj, char *key);
  int(*matchKeyToIndex)(struct Msgpack_base_unpacker_struct* self, msgpack_object parentObj, char *key);

} Msgpack_base_unpacker ;


/**
 * Creates a msgpack_parser instance.
 */
Msgpack_base_unpacker* msgpack_base_unpacker_create();
msgpack_object msgpack_parser_match_key_to_obj(Msgpack_base_unpacker* self, msgpack_object parentObj, char *key);
void msgpack_unpacker_copy_str(Msgpack_base_unpacker* self, char *str, char *data, int size);
void msgpack_parser_print_obj(Msgpack_base_unpacker* self, msgpack_object obj);
msgpack_object msgpack_parser_match_key_to_obj(Msgpack_base_unpacker* self, msgpack_object parentObj, char *key);
int msgpack_parser_match_key_to_index(Msgpack_base_unpacker* self, msgpack_object parentObj, char *key);

#endif // msgpack_parser_H_
