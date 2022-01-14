#include <gtest/gtest.h>
#include <string>
#include <gmock/gmock-matchers.h>
#include <Communication/FramedIOTestFunctions.h>

extern "C" {
#include <stream/MockByteStream.h>
#include <Scope/Core/Scope.h>
#include <Scope/Communication/Interfaces/FramedIO.h>
#include <Scope/Builders/ScopeFramedStack.h>
#include <Observer/ObserverMock.h>
#include <Scope/Serialisation/Protobuf/NanopbPacker.h>
#include <se-lib-c/stream/BufferedByteStream.h>
#include <se-lib-c/stream/BufferedIntStream.h>
#include <se-lib-c/stream/BufferedFloatStream.h>
#include <Scope/Control/AnnounceStorage.h>
#include <Scope/Builders/ScopeBuilder.h>
#include <Scope/Serialisation/Serializer.h>
}

using namespace testing;
using namespace std;

class FullIntegrationTest : public ::testing::Test{
#define MAX_NUMBER_OF_CHANNELS (3)
#define SIZE_OF_CHANNELS (50)
#define ADDRESSES_TO_ANNOUNCE (5)
#define TIMEBASE 1.5f

protected:
    FullIntegrationTest()
            : Test(){
    }

    void SetUp() override{

        ScopeFramedStackConfig config;
        config.timestamp = &_timestamp;
        config.callback = NULL;
        config.amountOfChannels = MAX_NUMBER_OF_CHANNELS;
        config.timebase = TIMEBASE;
        config.addressesInAddressAnnouncer = ADDRESSES_TO_ANNOUNCE;
        config.sizeOfChannels = SIZE_OF_CHANNELS;

        MockByteStream_init(&_mockBytestream);

        ScopeFramedStackLogOptions logOptions = {
                .logByteStream = &_mockBytestream.parent,
        };

        Message_Priorities priorities = {
                .data = HIGH,
                .log = MEDIUM,
                .stream = LOW,
        };

        _stack = ScopeFramedStack_create(config, logOptions, priorities);
        _transceiver = ScopeFramedStack_getTranscevier(_stack);
    }

    void TearDown() override{
    }

    ~FullIntegrationTest() override{
    }

    MockByteStream _mockBytestream;
    ScopeFramedStackHandle _stack;
    uint32_t _timestamp;
    ITransceiverHandle _transceiver;
};


TEST_F(FullIntegrationTest, normal_run
){

    vector<uint8_t> v_i;
    vector<uint8_t> v_d;
    uint8_t a_o[1000];
    vector<uint8_t> v_o;
    size_t a_o_s;

    v_i = prepareMessage(EV_DETECT, v_d);
    _transceiver->put(_transceiver, (uint8_t*) &v_i[0], v_i.size());
    ScopeFramedStack_run(_stack);
    a_o_s = _transceiver->outputSize(_transceiver);
    _transceiver->get(_transceiver, a_o, a_o_s);
    v_o.assign(a_o, a_o + a_o_s);
    EXPECT_THAT(extractMessageType(v_o), SC_DETECT);

    v_i = prepareMessage(SE_ACK, v_d);
    _transceiver->put(_transceiver, (uint8_t*) &v_i[0], v_i.size());
    ScopeFramedStack_run(_stack);

    v_i = prepareMessage(EV_ANNOUNCE, v_d);
    _transceiver->put(_transceiver, (uint8_t*) &v_i[0], v_i.size());
    ScopeFramedStack_run(_stack);
    a_o_s = _transceiver->outputSize(_transceiver);
    _transceiver->get(_transceiver, a_o, a_o_s);
    v_o.assign(a_o, a_o + a_o_s);
    EXPECT_THAT(extractMessageType(v_o), SC_ANNOUNCE);

    v_i = prepareMessage(SE_ACK, v_d);
    _transceiver->put(_transceiver, (uint8_t*) &v_i[0], v_i.size());
    ScopeFramedStack_run(_stack);

    // CF_RUNNING, channeld 1 = false, channeld 2 = false,
    v_d = {10, 2, 8, 1, 10, 0};
    v_i = prepareMessage(CF_RUNNING, v_d);
    _transceiver->put(_transceiver, (uint8_t*) &v_i[0], v_i.size());
    ScopeFramedStack_run(_stack);
    a_o_s = _transceiver->outputSize(_transceiver);
    _transceiver->get(_transceiver, a_o, a_o_s);
    v_o.assign(a_o, a_o + a_o_s);
    EXPECT_THAT(extractMessageType(v_o), SE_ACK);

}
