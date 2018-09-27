/*!****************************************************************************************************************************************
 * @file         msgpack_unpacker.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Author <Email>
 *
 * @brief        TODO
 *
 *****************************************************************************************************************************************/

#include <msgpack_parser/msgpack_base_unpacker.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

Msgpack_base_unpacker* msgpack_base_unpacker_create(){

    Msgpack_base_unpacker* self = (Msgpack_base_unpacker*) malloc(sizeof(Msgpack_base_unpacker));

    /* Matches functions */
    self->cpyStr = msgpack_unpacker_copy_str;
    self->printObj = msgpack_base_unpacker_print_obj;
    self->matchKeyToObj = msgpack_base_unpacker_match_key_to_obj;
    self->matchKeyToIndex = msgpack_base_unpacker_match_key_to_index;
    self->getIntFromObj = msgpack_base_unpacker_get_int_from_obj;
    self->getFloatFromObj = msgpack_base_unpacker_get_float_from_obj;
    self->getBoolFromObj = msgpack_base_unpacker_get_bool_from_obj;
    self->getStringFromObj = msgpack_base_unpacker_get_string_from_obj;

    return self;
}


/* Copies the msgpack_object key to key */
void msgpack_unpacker_copy_str(char *str, char *data, int size){
  strncpy(str, data, size);
  str[size + 1] = '\0';
}

/* Matches the given key to a object on the child level of the gives parent object
    returns a integer indicating the position of the key in the object */
int msgpack_base_unpacker_match_key_to_index(msgpack_object parentObj, char *key){
  /* Extract keys for every command */
  for(int i = 0; i<parentObj.via.map.size; i++){
    msgpack_object objKey = (parentObj.via.map.ptr+i)->key;

    char tmpKey[objKey.via.str.size + 1];
    msgpack_unpacker_copy_str(tmpKey, (char *)objKey.via.str.ptr, objKey.via.str.size);

    if(strncmp(tmpKey, key, objKey.via.str.size) == 0){
      return i;
    }
  }
  return -1;
}

/* Returns msgpack_object matching the the key */
msgpack_object msgpack_base_unpacker_match_key_to_obj(msgpack_object parentObj, char *key){

  int offset = msgpack_base_unpacker_match_key_to_index(parentObj, key);
  msgpack_object obj;

  if(offset >= 0 ){
    return (parentObj.via.map.ptr+offset)->val;
  }
  return obj;
}

int msgpack_base_unpacker_get_int_from_obj(msgpack_object obj, int offset){
  return (obj.via.map.ptr+offset)->val.via.i64;
}


float msgpack_base_unpacker_get_float_from_obj(msgpack_object obj, int offset){
  return (obj.via.map.ptr+offset)->val.via.f64;
}


bool msgpack_base_unpacker_get_bool_from_obj(msgpack_object obj, int offset){
  return (obj.via.map.ptr+offset)->val.via.boolean;
}

void msgpack_base_unpacker_get_string_from_obj(msgpack_object obj, int offset, char *targetStr, int maxLenght){

  int strLength = (obj.via.map.ptr+offset)->val.via.str.size;

  if(maxLenght + 1 >= strLength){
    strncpy(targetStr, (obj.via.map.ptr+offset)->val.via.str.ptr, strLength);
    targetStr[strLength + 1] = '\0';
  } else {
    targetStr[0] = '\0';
  }
}

void msgpack_base_unpacker_print_obj(msgpack_object obj){
    msgpack_object_print(stdout, obj);
}
