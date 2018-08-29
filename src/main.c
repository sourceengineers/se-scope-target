
#include <msgpack_parser/msgpack_parser.h>
#include <stdio.h>

int main()
{


    Msgpack_parser parser = msgpack_parser_create();
    parser.unpack(&parser, "Hello", 3);

    return 0;
}
