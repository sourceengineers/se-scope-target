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


//#include "Shared/types.h"

typedef struct Msgpack_parser_struct
{
  OutputData outputData;
  InputData inputData;
  msgpack_unpacker unp;
  void(*unpack)(struct Msgpack_parser_struct* self, char* data, int length);
} Msgpack_parser ;


/**
 * Creates a msgpack_parser instance.
 */
Msgpack_parser msgpack_parser_create();

void msgpack_parser_unpack(Msgpack_parser* self, char *data, int length);

#endif // msgpack_parser_H_
