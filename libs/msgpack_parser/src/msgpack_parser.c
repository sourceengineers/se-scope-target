/*!****************************************************************************************************************************************
 * @file         msgpack_parser.c
 *
 * @copyright    Copyright (c) 2018 by Sonnen. All Rights Reserved.
 *
 * @authors      Author <Email>
 *
 * @brief        TODO
 *
 *****************************************************************************************************************************************/

#include <msgpack_parser/msgpack_parser.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

Msgpack_parser* msgpack_parser_create(){//_create(Shared_Memory_MemoryAllocator* allocator)
    /*
    PrivateData* me = (PrivateData*)allocator->allocate(allocator->handle, sizeof(PrivateData));
    ASSERT(me != NULL);
*/
    OutputData outputData;
    InputData inputData;


    Msgpack_parser* parser = (Msgpack_parser*) malloc(sizeof(Msgpack_parser));


    /* Initialize output buffers */
    msgpack_sbuffer_init(&outputData.sbuf); /* initialize buffer */
    msgpack_packer_init(&outputData.pk, &outputData.sbuf, msgpack_sbuffer_write); /* initialize packer */


    /* Initialize input buffers */
    bool result = msgpack_unpacker_init(&parser->unp, 100);
    msgpack_unpacked_init(&inputData.und);

    parser->outputData = outputData;
    parser->inputData = inputData;
    parser->unpack = msgpack_parser_unpack;
    parser->print_obj = msgpack_parser_print_obj;

    return parser;
}

void msgpack_parser_unpack(Msgpack_parser* self, char *data, int request_size){

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


void msgpack_copy_key(char *key, char *data, int size){

  for (int i = 0; i < size; i++) {
    key[i] = data[i];
  }
  key[size] = "\0";
}

void msgpack_parser_print_obj(Msgpack_parser* self){

    msgpack_object payload =  (self->inputData.obj.via.map.ptr+1)->val;
    msgpack_object sc_cmd =  payload.via.map.ptr->val;



    /* Extract keys for every command */
    for(int i = 0; i<sc_cmd.via.map.size; i++){
      char key[50];

      msgpack_object cmd_key = (sc_cmd.via.map.ptr+i)->key;
      msgpack_copy_key(key, cmd_key.via.str.ptr,cmd_key.via.str.size);

    }



    //msgpack_object cmd_obj = sc_cmd.via.map.ptr->val;


    msgpack_object_print(stdout, sc_cmd);

}

void msgpack_parser_get_cmd_data(Msgpack_parser* self, char* cmdName){

    msgpack_object cmdData =  (self->inputData.obj.via.map.ptr+1)->val;

    msgpack_object_print(stdout, cmdData);

}

void msgpack_parser_pack(Msgpack_parser* self){

}
