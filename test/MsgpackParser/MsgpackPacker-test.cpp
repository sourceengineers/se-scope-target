#include <gtest/gtest.h>

extern "C" {
    #include <Scope/MsgpackParser/MsgpackPacker.h>
    #include <Scope/Communication/CommunicationCommons.h>
    #include <Scope/GeneralPurpose/IntStream.h>
}

using namespace testing;
using namespace std;

TEST(msgpack_packer, pack_test)
{

  IByteStreamHandle byteStream = ByteStream_getIByteStream(ByteStream_create(200));

  MsgpackPackerHandle packer = MsgpackPacker_create(200, 2, 0, byteStream, NULL);
  IPackerHandle iPacker = MsgpackPacker_getIPacker(packer);

  FloatRingBufferHandle ringbuffers[3];
  IFloatStreamHandle streams[2];
  IIntStreamHandle timestamp;


  for (int i = 0; i < 3; ++i) {
    ringbuffers[i] = FloatRingBuffer_create(100);
    streams[i] = FloatRingBuffer_getIFloatStream(ringbuffers[i]);
    const float writeData[5] = {1.5f, 2.0f, 432.0f, 32.0f, 45.0f};
    FloatRingBuffer_write(ringbuffers[i], writeData, 5);
  }
  timestamp = IntStream_getIIntStream(IntStream_create(100));

  iPacker->prepareTrigger(iPacker, true, 2, 50);
  iPacker->prepareTimeIncrement(iPacker, 10);
  iPacker->prepareTimestamp(iPacker, timestamp);
  iPacker->prepareFlowControl(iPacker, (const char*) "ACK");
  iPacker->prepareChannel(iPacker, streams[0], 0);
  iPacker->prepareChannel(iPacker, streams[1], 1);

  iPacker->pack(iPacker);
}
