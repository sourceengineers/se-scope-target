#include <gtest/gtest.h>

extern "C" {
    #include "MsgpackPacker.h"
}

using namespace testing;


TEST(msgpack_packer, pack_test)
{

  IByteStreamHandle byteStream = ByteStream_getByteStream(ByteStream_create(200));

  MsgpackPackerHandle packer = MsgpackPacker_create(200, 2, byteStream);
  IPackerHandle iPacker = MsgpackPacker_getIPacker(packer);

  FloatRingBufferHandle ringbuffers[2];
  IFloatStreamHandle streams[2];

  for (int i = 0; i < 2; ++i) {
    ringbuffers[i] = FloatRingBuffer_create(100);
    streams[i] = FloatRingBuffer_getFloatStream(ringbuffers[i]);
    const float writeData[5] = {1.5f, 2.0f, 432.0f, 32.0f, 45.0f};
    FloatRingBuffer_write(ringbuffers[i], writeData, 5);
  }

  iPacker->prepareTrigger(iPacker, true, 2, 50);
  iPacker->prepareTimeIncrement(iPacker, 10);
  iPacker->prepareTimestamp(iPacker, 100);
  iPacker->prepareFlowControl(iPacker, (const char*) "ACK");
  iPacker->prepareChannel(iPacker, streams[0], 0);
  iPacker->prepareChannel(iPacker, streams[1], 1);

  iPacker->pack(iPacker);
}
