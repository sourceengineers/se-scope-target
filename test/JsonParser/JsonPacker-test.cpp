#include <gtest/gtest.h>

extern "C" {
#include <Scope/Serialisation/JsonParser/JsonPacker.h>
#include <Scope/GeneralPurpose/BufferedByteStream.h>
#include <Scope/GeneralPurpose/BufferedIntStream.h>
#include <Scope/GeneralPurpose/BufferedFloatStream.h>
}

using namespace testing;
using namespace std;

TEST(json_packer, unpack_test){

    size_t channelSize = 400;
    size_t sizes = JsonPacker_calculateBufferSize(5, 10, channelSize);

    float value;

    IByteStreamHandle outputStream = BufferedByteStream_getIByteStream(BufferedByteStream_create(sizes));

    IIntStreamHandle timestamp = BufferedIntStream_getIIntStream(BufferedIntStream_create(channelSize));
    FloatRingBufferHandle buf1 = FloatRingBuffer_create(channelSize);
    FloatRingBufferHandle buf2 = FloatRingBuffer_create(channelSize);

    IPackerHandle packer = JsonPacker_getIPacker(JsonPacker_create(5, 10, outputStream));

    packer->prepareAddressAnnouncement(packer, "VAR_1", "UINT32", 11111);
    packer->prepareAddressAnnouncement(packer, "VAR_2", "FLOAT", 22222);

    packer->prepareTrigger(packer, true, 1, 1000);

    packer->prepareTimeIncrement(packer, 10);
    packer->prepareFlowControl(packer, "ACK");

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

    packer->prepareTimestamp(packer, timestamp);

    packer->prepareChannel(packer, buf1, 0);
    packer->prepareChannel(packer, buf2, 1);

    packer->pack(packer);

    size_t dataPending = outputStream->length(outputStream);
    char data[dataPending + 1];

    outputStream->read(outputStream, (uint8_t*) data, dataPending);
    data[dataPending] = '\0';

    EXPECT_STREQ(data,
                 "{\"payload\":{\"sc_data\":{\"cl_data\":{\"0\":[-23.54,2.345e+08,3.45e-06,-3.45e-06,0,1.5],\"1\":[-23.54,2.345e+08,3.45e-06,-3.45e-06,0,1.5]},\"t_stmp\":[0,1,2,3,4,5],\"t_inc\":10,\"tgr\":{\"found\":true,\"cl_data_ind\":1000,\"cl_id\":1},\"sc_announce\":{\"VAR_1\":[11111,\"UINT32\"],\"VAR_2\":[22222,\"FLOAT\"],\"cl_amount\":5}},\"flow_ctrl\":\"ACK\"}}");

}
