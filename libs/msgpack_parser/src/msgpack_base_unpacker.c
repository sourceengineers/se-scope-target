/*!****************************************************************************************************************************************
 * @file         msgpack_unpacker.c
 *
 * @copyright    Copyright (c) 2018 by Sonnen. All Rights Reserved.
 *
 * @authors      Author <Email>
 *
 * @brief        TODO
 *
 *****************************************************************************************************************************************/

#include <msgpack_parser/msgpack_unpacker.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

Msgpack_base_unpacker* msgpack_base_unpacker_create(){//_create(Shared_Memory_MemoryAllocator* allocator)
    
    Msgpack_base_unpacker* self = (Msgpack_base_unpacker*) malloc(sizeof(Msgpack_base_unpacker));

    self->matchObjToKey = msgpack_parser_match_key_to_obj;
    self->cpyStr = msgpack_unpacker_copy_str;
    self->printObj = msgpack_parser_print_obj;
    self->matchKeyToObj = msgpack_parser_match_key_to_obj;
    self->matchKeyToIndex = msgpack_parser_match_key_to_index;

    return self;
}


/* Copies the msgpack_object key to key */
void msgpack_unpacker_copy_str(char *str, char *data, int size){
  strncpy(str, data, size);
  str[size + 1] = '\0';
}

/* Matches the given key to a object on the child level of the gives parent object 
    returns a integer indicating the position of the key in the object */
int msgpack_parser_match_key_to_index(Msgpack_parser* self, msgpack_object parentObj, char *key){
  /* Extract keys for every command */
  for(int i = 0; i<parentObj.via.map.size; i++){
    msgpack_object objKey = (parentObj.via.map.ptr+i)->key;
    
    char tmpKey[objKey.via.str.size + 1];
    msgpack_unpacker_copy_key(tmpKey, objKey.via.str.ptr, objKey.via.str.size);
    
    if(strncmp(tmpKey, key, objKey.via.str.size) == 0){
      return i;
    }
  }  
  return -1;
}

/* Returns msgpack_object matching the the key */
msgpack_object msgpack_parser_match_key_to_obj(Msgpack_parser* self, msgpack_object parentObj, char *key){

  int offset = msgpack_parser_match_key_to_index(self, parentObj, key);
  msgpack_object obj;
  
  if(offset >= 0 ){
    return (parentObj.via.map.ptr+offset)->val;
  }
  return obj;
}

int msgpack_unpacker_get_int_from_map(msgpack_object obj, int offset){
  
}


void msgpack_base_unpacker_print_obj(Msgpack_parser* self, msgpack_object obj){
    msgpack_object_print(stdout, obj);
}
