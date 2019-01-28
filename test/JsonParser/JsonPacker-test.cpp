#include <gtest/gtest.h>

extern "C" {
#include <Scope/Serialisation/JsonParser/JsonPacker.h>
#include <Scope/GeneralPurpose/BufferedByteStream.h>
#include <Scope/GeneralPurpose/BufferedIntStream.h>
#include <Scope/GeneralPurpose/BufferedFloatStream.h>
}

using namespace testing;
using namespace std;

IComValidator validator;

static bool validateCheck(IComValidatorHandle self, const uint8_t *check, const size_t lengthOfCheck,
                          const uint8_t *bytesToCheck, const size_t lengthOfBytesToCheck) {
    return true;
}

static size_t getCheckLength(IComValidatorHandle self) {
    return 4;
}

static void createCheck(IComValidatorHandle self, uint8_t *checksum, const uint8_t *bytesToCheck, const size_t length) {

    for (int i = 0; i < length; ++i) {
        checksum[i % 4] += bytesToCheck[i];
    }
}


static bool checkPresentInProtocol(IComValidatorHandle self) {
    return false;
}

TEST(json_packer, unpack_test) {

    size_t channelSize = 400;
    size_t sizes = JsonPacker_calculateBufferSize(5, 10, channelSize);

    float d1, d2;

    IByteStreamHandle outputStream = BufferedByteStream_getIByteStream(BufferedByteStream_create(sizes));

    IIntStreamHandle timestamp = BufferedIntStream_getIIntStream(BufferedIntStream_create(channelSize));
    ChannelHandle ch1 = Channel_create(channelSize);
    ChannelHandle ch2 = Channel_create(channelSize);

    Channel_setPollAddress(ch1, &d1, FLOAT);
    Channel_setPollAddress(ch2, &d2, FLOAT);
    Channel_setStateRunning(ch1);
    Channel_setStateRunning(ch2);

    IPackerHandle packer = JsonPacker_getIPacker(JsonPacker_create(5, 10,  outputStream));

    packer->prepareAddressAnnouncement(packer, "VAR_1", "UINT32", 11111);
    packer->prepareAddressAnnouncement(packer, "VAR_2", "FLOAT", 22222);

    packer->prepareTrigger(packer, true, 1, 1000);

    packer->prepareTimeIncrement(packer, 10);
    packer->prepareFlowControl(packer, "ACK");

    d1 = (const float) -23.5354;
    d2 = (const float) -23.5354;
    Channel_poll(ch1);
    Channel_poll(ch2);
    timestamp->writeData(timestamp, 0);

    d1 = (const float) 234534523.5354;
    d2 = (const float) 234534523.5354;
    Channel_poll(ch1);
    Channel_poll(ch2);
    timestamp->writeData(timestamp, 1);

    d1 = (const float) 0.00000345;
    d2 = (const float) 0.00000345;
    Channel_poll(ch1);
    Channel_poll(ch2);
    timestamp->writeData(timestamp, 2);

    d1 = (const float) -0.00000345;
    d2 = (const float) -0.00000345;
    Channel_poll(ch1);
    Channel_poll(ch2);
    timestamp->writeData(timestamp, 3);

    d1 = (const float) 0;
    d2 = (const float) 0;
    Channel_poll(ch1);
    Channel_poll(ch2);
    timestamp->writeData(timestamp, 4);

    d1 = (const float) 1.5;
    d2 = (const float) 1.5;
    Channel_poll(ch1);
    Channel_poll(ch2);
    timestamp->writeData(timestamp, 5);

    packer->prepareTimestamp(packer, timestamp);

    size_t length_ch1 = Channel_getAmountOfUsedData(ch1);
    float data_ch1[length_ch1];
    Channel_read(ch1, data_ch1, length_ch1);

    size_t length_ch2 = Channel_getAmountOfUsedData(ch2);
    float data_ch2[length_ch2];
    Channel_read(ch2, data_ch2, length_ch2);

    packer->prepareChannel(packer, data_ch1, length_ch1, 0);
    packer->prepareChannel(packer, data_ch2, length_ch2, 1);

    packer->pack(packer);

    size_t dataPending = outputStream->length(outputStream);
    char data[dataPending];

    outputStream->read(outputStream, (uint8_t *) data, dataPending);

    EXPECT_STREQ(data,
                 "\"payload\":{\"sc_data\":{\"cl_data\":{\"0\":[-23.54,2.345e+08,3.45e-06,-3.45e-06,0,1.5],\"1\":[-23.54,2.345e+08,3.45e-06,-3.45e-06,0,1.5]},\"t_stmp\":[0,1,2,3,4,5],\"t_inc\":10,\"tgr\":{\"found\":true,\"cl_data_ind\":1000,\"cl_id\":1},\"sc_announce\":{\"VAR_1\":[11111,\"UINT32\"],\"VAR_2\":[22222,\"FLOAT\"],\"cl_amount\":5}},\"flow_ctrl\":\"ACK\"}");

    printf("Output: %s", data);
}
