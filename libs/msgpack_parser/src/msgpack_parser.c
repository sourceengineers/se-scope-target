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

Msgpack_parser msgpack_parser_create(){//_create(Shared_Memory_MemoryAllocator* allocator)
    /*
    PrivateData* me = (PrivateData*)allocator->allocate(allocator->handle, sizeof(PrivateData));
    ASSERT(me != NULL);
*/
    Msgpack_parser parser;

    OutputData outputData;
    InputData inputData;
    inputData.dataSize = INPUT_DATA_SIZE;
    for(int i = 0; i<inputData.dataSize; i++){
      inputData.data[i] = 0;
    }

    /* Initialize output buffers */
    msgpack_sbuffer_init(&outputData.sbuf); /* initialize buffer */
    msgpack_packer_init(&outputData.pk, &outputData.sbuf, msgpack_sbuffer_write); /* initialize packer */


    /* Initialize input buffers */
    bool result = msgpack_unpacker_init(&parser.unp, 100);
    msgpack_unpacked_init(&inputData.und);

    parser.outputData = outputData;
    parser.inputData = inputData;
    parser.unpack = msgpack_parser_unpack;

    return parser;
}

void msgpack_parser_unpack(Msgpack_parser* self, char *data, int length){

  strcpy(self->inputData.data,data);

  printf(self->inputData.data);
}
