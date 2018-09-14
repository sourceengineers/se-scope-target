#include <gtest/gtest.h>

extern "C" {
    #include "ring_buffer.h"
}


TEST(msgpack_unpacker, unpack_cmd_cf_tgr)
{
  size_t setCapacity = 10;

  RingBuffer* buffer = RingBuffer_create(setCapacity);
  size_t getCapacity = getCapacity(buffer);
  EXPECT_EQ(getCapacity, setCapacity);

  


}
