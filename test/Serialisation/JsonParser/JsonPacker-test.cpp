#include <gtest/gtest.h>
#include <string>

extern "C" {
#include <Scope/Serialisation/JsonParser/JsonPacker.h>
#include <Scope/GeneralPurpose/BufferedByteStream.h>
#include <Scope/GeneralPurpose/BufferedIntStream.h>
#include <Scope/GeneralPurpose/BufferedFloatStream.h>
}

using namespace testing;
using namespace std;

class JsonPackerTest : public ::testing::Test{
#define MAX_NUMBER_OF_CHANNELS (3)
#define SIZE_OF_CHANNELS (10)
#define ADDRESSES_TO_ANNOUNCE (2)

protected:
    JsonPackerTest()
            : Test(){
    }

    void SetUp() override{
        size_t channelSize = 400;
        size_t sizes = JsonPacker_calculateBufferSize(MAX_NUMBER_OF_CHANNELS, SIZE_OF_CHANNELS, ADDRESSES_TO_ANNOUNCE);
        float value;
        _outputStream = BufferedByteStream_getIByteStream(BufferedByteStream_create(sizes));
        _packer = JsonPacker_create(MAX_NUMBER_OF_CHANNELS, ADDRESSES_TO_ANNOUNCE, _outputStream);
        _iPacker = JsonPacker_getIPacker(_packer);
    }

    void TearDown() override{
    }

    string getOutput(){
        size_t dataPending = _outputStream->length(_outputStream);
        char data[dataPending + 1];
        _outputStream->read(_outputStream, (uint8_t*) data, dataPending);
        data[dataPending] = '\0';
        string str(data);
        return str;
    }

    ~JsonPackerTest() override{
        JsonPacker_destroy(_packer);
    }

    JsonPackerHandle _packer;
    IPackerHandle _iPacker;
    IByteStreamHandle _outputStream;
};


TEST_F(JsonPackerTest, pack_announce){

    _iPacker->addAddressAnnouncement(_iPacker, "test_var_1", "SE_FLOAT", 0xFFFF);
    _iPacker->addAddressAnnouncement(_iPacker, "test_var_2", "SE_FLOAT", 0xaaaa);
    _iPacker->addAnnouncement(_iPacker, 1.5f, "0.3", MAX_NUMBER_OF_CHANNELS);
    _iPacker->pack(_iPacker, SC_ANNOUNCE);

    EXPECT_EQ(getOutput(),
              "{\"sc_announce\":{\"cl_amount\":3,\"version\":\"0.3\",\"time_base\":1.5,\"channels\":{\"test_var_1\":[65535,\"SE_FLOAT\"],\"test_var_2\":[43690,\"SE_FLOAT\"]}}}");
}

TEST_F(JsonPackerTest, pack_data){
//    IIntStreamHandle timestamp = BufferedIntStream_getIIntStream(BufferedIntStream_create(SIZE_OF_CHANNELS));
//    FloatRingBufferHandle buf1 = FloatRingBuffer_create(SIZE_OF_CHANNELS);
//    FloatRingBufferHandle buf2 = FloatRingBuffer_create(SIZE_OF_CHANNELS);
//
//    _iPacker->addTrigger(_iPacker, true, 1, 1000, (char*) "Normal");
//    _iPacker->addTimeIncrement(_iPacker, 10);
//
//    float value;
//    value = -23.5354f;
//    FloatRingBuffer_write(buf1, &value, 1);
//    FloatRingBuffer_write(buf2, &value, 1);
//    timestamp->writeData(timestamp, 0);
//    value = 234534523.5354f;
//    FloatRingBuffer_write(buf1, &value, 1);
//    FloatRingBuffer_write(buf2, &value, 1);
//    timestamp->writeData(timestamp, 1);
//    value = 0.00000345;
//    FloatRingBuffer_write(buf1, &value, 1);
//    FloatRingBuffer_write(buf2, &value, 1);
//    timestamp->writeData(timestamp, 2);
//    value = -0.00000345f;
//    FloatRingBuffer_write(buf1, &value, 1);
//    FloatRingBuffer_write(buf2, &value, 1);
//    timestamp->writeData(timestamp, 3);
//    value = 0.0f;
//    FloatRingBuffer_write(buf1, &value, 1);
//    FloatRingBuffer_write(buf2, &value, 1);
//    timestamp->writeData(timestamp, 4);
//    value = 1.5f;
//    FloatRingBuffer_write(buf1, &value, 1);
//    FloatRingBuffer_write(buf2, &value, 1);
//    timestamp->writeData(timestamp, 5);
//    _iPacker->addTimestamp(_iPacker, timestamp);
//    _iPacker->addChannel(_iPacker, buf1, 0);
//    _iPacker->addChannel(_iPacker, buf2, 1);
//    _iPacker->pack(_iPacker, SC_DATA);
//    EXPECT_EQ(getOutput(), "{\"sc_data\":{\"cl_data\":{\"0\":[-23.54,2.345e+08,3.45e-06,-3.45e-06,0,1.5],\"1\":[-23.54,2.345e+08,3.45e-06,-3.45e-06,0,1.5]},\"t_stmp\":[0,1,2,3,4,5],\"t_inc\":10,\"tgr\":{\"found\":true,\"cl_data_ind\":1000,\"mode\":\"Normal\",\"cl_id\":1}}}");
}