#include <msgpack.h>


#define INPUT_DATA_SIZE 300

typedef struct {
  msgpack_unpacked und;   /* unpacked data */
  msgpack_unpack_return ret; /* unpacking return data */
  msgpack_object obj;
} InputData;

typedef struct {
  msgpack_sbuffer sbuf; /* buffer */
  msgpack_packer pk;    /* packer */
} OutputData;
