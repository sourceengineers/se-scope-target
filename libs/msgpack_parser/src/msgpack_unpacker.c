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

Msgpack_parser* msgpack_unpacker_create(){//_create(Shared_Memory_MemoryAllocator* allocator)
    
    InputData inputData;
    Msgpack_parser* parser = (Msgpack_parser*) malloc(sizeof(Msgpack_parser));

    /* Initialize input buffers */
    bool result = msgpack_unpacker_init(&parser->unp, 100);
    msgpack_unpacked_init(&inputData.und);

    /* Map functions */
    parser->inputData = inputData;
    parser->unpack = msgpack_unpacker_unpack;
    parser->getCmdObj = msgpack_parser_getCmdObj;
    parser->printObj = msgpack_parser_print_obj;

    return parser;
}

/* Unpacks the data and safes it in self->inputData */
void msgpack_unpacker_unpack(Msgpack_parser* self, char *data, int request_size){

  if (msgpack_unpacker_buffer_capacity(&self->unp) < request_size) {
    bool result = msgpack_unpacker_reserve_buffer(&self->unp, request_size);
    if (result) {
        memcpy(msgpack_unpacker_buffer(&self->unp), data, request_size);
        msgpack_unpacker_buffer_consumed(&self->unp, request_size);


        self->inputData.ret = msgpack_unpacker_next(&self->unp, &self->inputData.und);
        switch(self->inputData.ret) {
        case MSGPACK_UNPACK_SUCCESS:
            {
                self->inputData.obj = self->inputData.und.data;
                /* Extract msgpack_object and use it. */
            }
            break;
        case MSGPACK_UNPACK_CONTINUE:
            /* cheking capacity, reserve buffer, copy additional data to the buffer, */
            /* notify consumed buffer size, then call msgpack_unpacker_next(&unp, &upd) again */
            break;
        case MSGPACK_UNPACK_PARSE_ERROR:
            /* Error process */
            break;
        }
    }

  }
}

/* Copies the msgpack_object key to key */
void msgpack_unpacker_copy_key(char *key, char *data, int size){
  strncpy(key, data, size);
  key[size + 1] = '\0';
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

msgpack_object msgpack_parser_getCmdObj(Msgpack_parser* self, char *key){
    msgpack_object obj;
    obj = msgpack_parser_match_key_to_obj(self, self->inputData.obj, "payload");
    obj = msgpack_parser_match_key_to_obj(self, obj, "sc_cmd");
    return msgpack_parser_match_key_to_obj(self, obj, key);
}


int msgpack_unpacker_get_int_from_map(msgpack_object obj, int offset){
  
}


void msgpack_parser_print_obj(Msgpack_parser* self, msgpack_object obj){
    msgpack_object_print(stdout, obj);
}
