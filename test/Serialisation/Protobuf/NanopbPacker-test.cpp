#include <gtest/gtest.h>
#include <string>
#include <gmock/gmock-matchers.h>

extern "C" {
#include <Scope/Serialisation/Protobuf/NanopbPacker.h>
#include <se-lib-c/stream/BufferedByteStream.h>
#include <se-lib-c/stream/BufferedIntStream.h>
#include <se-lib-c/stream/BufferedFloatStream.h>
}

using namespace testing;
using namespace std;

class NanopbPackerTest : public ::testing::Test{
#define MAX_NUMBER_OF_CHANNELS (3)
#define SIZE_OF_CHANNELS (10)
#define ADDRESSES_TO_ANNOUNCE (3)

protected:
    NanopbPackerTest()
            : Test(){
    }

    void SetUp() override{
        size_t channelSize = 400;
        size_t sizes = NanopbPacker_calculateBufferSize(MAX_NUMBER_OF_CHANNELS, SIZE_OF_CHANNELS, ADDRESSES_TO_ANNOUNCE);
        float value;
        _outputStream = BufferedByteStream_getIByteStream(BufferedByteStream_create(sizes));
        _packer = NanopbPacker_create(MAX_NUMBER_OF_CHANNELS, ADDRESSES_TO_ANNOUNCE, _outputStream);
        _iPacker = NanopbPacker_getIPacker(_packer);
    }

    void TearDown() override{
    }

    ~NanopbPackerTest() override{
        NanopbPacker_destroy(_packer);
    }

    NanopbPackerHandle _packer;
    IPackerHandle _iPacker;
    IByteStreamHandle _outputStream;
};


TEST_F(NanopbPackerTest, pack_data
){

    IIntStreamHandle timestamp = BufferedIntStream_getIIntStream(BufferedIntStream_create(SIZE_OF_CHANNELS));
    FloatRingBufferHandle buf1 = FloatRingBuffer_create(SIZE_OF_CHANNELS);
    FloatRingBufferHandle buf2 = FloatRingBuffer_create(SIZE_OF_CHANNELS);
    float value;
    value = -23.5354f;
    FloatRingBuffer_write(buf1, &value, 1);
    FloatRingBuffer_write(buf2, &value, 1);
    timestamp->writeData(timestamp, 0);
    value = 234534523.5354f;
    FloatRingBuffer_write(buf1, &value, 1);
    FloatRingBuffer_write(buf2, &value, 1);
    timestamp->writeData(timestamp, 1);
    value = 0.00000345;
    FloatRingBuffer_write(buf1, &value, 1);
    FloatRingBuffer_write(buf2, &value, 1);
    timestamp->writeData(timestamp, 2);
    value = -0.00000345f;
    FloatRingBuffer_write(buf1, &value, 1);
    FloatRingBuffer_write(buf2, &value, 1);
    timestamp->writeData(timestamp, 3);
    value = 0.0f;
    FloatRingBuffer_write(buf1, &value, 1);
    FloatRingBuffer_write(buf2, &value, 1);
    timestamp->writeData(timestamp, 4);
    value = 1.5f;
    FloatRingBuffer_write(buf1, &value, 1);
    FloatRingBuffer_write(buf2, &value, 1);
    timestamp->writeData(timestamp, 5);

    ScDataChannelDef channel;
    channel.stream = buf1;
    channel.id = 0;
    _iPacker->addTimestamp(_iPacker, timestamp);
    _iPacker->addChannel(_iPacker, channel);
    channel.stream = buf2;
    channel.id = 1;
    _iPacker->addChannel(_iPacker, channel);

    ScDataTriggerDef trigger;
    trigger.isTriggered = true;
    trigger.triggerMode = TRIGGER_ONESHOT;
    trigger.channelId = 1;
    trigger.timestamp = 100;
    _iPacker->addTrigger(_iPacker, trigger);
    _iPacker->addTimeIncrement(_iPacker, 10);
    _iPacker->pack(_iPacker, SC_DATA);

    size_t dataPending = _outputStream->length(_outputStream);
    char data[dataPending];
    _outputStream->read(_outputStream, (uint8_t*) data, dataPending);

    vector<uint8_t> v_o;
    v_o.assign(data, data + dataPending);
    EXPECT_THAT(v_o, ElementsAre(10, 26, 10, 24, 128, 72, 188, 193, 104, 171, 95, 77, 140, 134, 103, 54, 140,
            134, 103, 182, 0, 0, 0, 0, 0, 0, 192, 63, 10, 28, 10, 24, 128, 72, 188, 193, 104, 171, 95, 77, 140, 134,
            103, 54, 140, 134, 103, 182, 0, 0, 0, 0, 0, 0, 192, 63, 16, 1, 18, 24, 0, 0, 0, 0, 1, 0, 0, 0, 2, 0, 0, 0,
            3, 0, 0, 0, 4, 0, 0, 0, 5, 0, 0, 0, 24, 10, 34, 6, 8, 1, 16, 100, 24, 2
    ));
}


TEST_F(NanopbPackerTest, pack_announce
){

    ScAnnounceChannelDef channel1;
    // Test if the max. amount of letters is properly written
    char* name1 = (char*) "aaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    channel1.name = name1;
    channel1.type = SE_INT8;
    channel1.id = 1;
    channel1.address = 100;

    ScAnnounceChannelDef channel2;
    char* name2 = (char*) "channel2";
    channel2.name = name2;
    channel2.type = SE_UINT32;
    channel2.id = 10;
    channel2.address = 200;

    ScAnnounceChannelDef channel3;
    char* name3 = (char*) "channel3";
    channel3.name = name3;
    channel3.type = SE_FLOAT;
    channel3.address = 0;
    channel3.id = 5;

    ScAnnounceMetaData meta;
    meta.maxChannels = 3;
    meta.timebase = 1.2345f;
    char* version = (char*) "0.5";
    meta.version = version;

    _iPacker->addAddressAnnouncement(_iPacker, channel1);
    _iPacker->addAddressAnnouncement(_iPacker, channel2);
    _iPacker->addAddressAnnouncement(_iPacker, channel3);
    _iPacker->addAnnouncement(_iPacker, meta);
    _iPacker->pack(_iPacker, SC_ANNOUNCE);

    size_t dataPending = _outputStream->length(_outputStream);
    char data[dataPending];
    _outputStream->read(_outputStream, (uint8_t*) data, dataPending);

    vector<uint8_t> v_o;
    v_o.assign(data, data + dataPending);
    EXPECT_THAT(v_o, ElementsAre(10, 35, 8, 1, 18, 29, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97,
            97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 32, 100, 10, 17, 8, 10, 18, 8, 99, 104, 97, 110,
            110, 101, 108, 50, 24, 5, 32, 200, 1, 10, 14, 8, 5, 18, 8, 99, 104, 97, 110, 110, 101, 108, 51, 24, 6, 16,
            3, 29, 25, 4, 158, 63, 34, 3, 48, 46, 53));
}
