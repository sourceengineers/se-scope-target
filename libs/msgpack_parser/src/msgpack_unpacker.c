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

Msgpack_unpacker* msgpack_unpacker_create(){//_create(Shared_Memory_MemoryAllocator* allocator)

    Msgpack_unpacker* self = (Msgpack_unpacker*) malloc(sizeof(Msgpack_unpacker));
    /* Creates the parent object */
    self->super = msgpack_base_unpacker_create();

    /* Initialize input buffers */
    bool result = msgpack_unpacker_init(&self->unp, 100);
    msgpack_unpacked_init(&self->und);

    /* Map functions */
    self->unpack = msgpack_unpacker_unpack;
    self->getCmdObj = Msgpack_unpacker_getCmdObj;

    /* Map inherited functions */
    self->cpyStr = self->super->cpyStr;
    self->printObj = self->super->printObj;
    self->matchKeyToObj = self->super->matchKeyToObj;
    self->matchKeyToIndex = self->super->matchKeyToIndex;
    self->getIntFromObj = self->super->getIntFromObj;
    self->getFloatFromObj = self->super->getFloatFromObj;
    self->getBoolFromObj = self->super->getBoolFromObj;
    self->getStringFromObj = self->super->getStringFromObj;

    return self;
}

/* Unpacks the data and safes it */
bool msgpack_unpacker_unpack(Msgpack_unpacker* self, char *data, int request_size){

  if (msgpack_unpacker_buffer_capacity(&self->unp) < request_size) {
    bool result = msgpack_unpacker_reserve_buffer(&self->unp, request_size);
    if (result) {
        memcpy(msgpack_unpacker_buffer(&self->unp), data, request_size);
        msgpack_unpacker_buffer_consumed(&self->unp, request_size);


        self->ret = msgpack_unpacker_next(&self->unp, &self->und);
        switch(self->ret) {
        case MSGPACK_UNPACK_SUCCESS:
            {
                self->obj = self->und.data;
                return true;
            }
            break;
        case MSGPACK_UNPACK_CONTINUE:
            /* cheking capacity, reserve buffer, copy additional data to the buffer, */
            /* notify consumed buffer size, then call msgpack_unpacker_next(&unp, &upd) again */
            return false;
            break;
        case MSGPACK_UNPACK_PARSE_ERROR:
          return false;
            break;
        }
    }

  }
}

/* Returns the msgpack_object corresponding to the given key */
msgpack_object Msgpack_unpacker_getCmdObj(Msgpack_unpacker* self, char *key){
    msgpack_object obj;
    obj = self->super->matchKeyToObj(self->obj, "payload");
    obj = self->super->matchKeyToObj(obj, "sc_cmd");
    return self->super->matchKeyToObj(obj, key);
}
