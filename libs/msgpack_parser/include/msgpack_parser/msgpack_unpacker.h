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


typedef struct Msgpack_parser_struct
{
  OutputData outputData;
  InputData inputData;
  msgpack_unpacker unp;
  void(*unpack)(struct Msgpack_parser_struct* self, char* data, int length);
  void(*printObj)(struct Msgpack_parser_struct* self, msgpack_object obj);
  msgpack_object(*getCmdObj)(struct Msgpack_parser_struct* self, char* key);

} Msgpack_parser ;


/**
 * Creates a msgpack_parser instance.
 */
Msgpack_parser* msgpack_unpacker_create();

void msgpack_unpacker_unpack(Msgpack_parser* self, char *data, int request_size);

msgpack_object msgpack_parser_getCmdObj(Msgpack_parser* self, char *key);

#endif // msgpack_parser_H_
