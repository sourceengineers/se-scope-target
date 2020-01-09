#include <gtest/gtest.h>
#include <string>
#include <gmock/gmock-matchers.h>

extern "C" {
#include <Scope/Serialisation/Protobuf/NanopbUnpacker.h>
#include <se-lib-c/stream/BufferedByteStream.h>
#include <se-lib-c/stream/BufferedIntStream.h>
#include <se-lib-c/stream/BufferedFloatStream.h>
}

using namespace testing;
using namespace std;

class NanopbUnpackerTest : public ::testing::Test{
#define INPUT_STREAM_SIZE (200)
#define MAX_CHANNELS (3)

protected:
    NanopbUnpackerTest()
            : Test(){
    }

    void SetUp() override{
        float value;
        _input = BufferedByteStream_getIByteStream(BufferedByteStream_create(INPUT_STREAM_SIZE));
        _unpacker = NanopbUnpacker_create(_input, MAX_CHANNELS);
        _iUnpacker = NanopbUnpacker_getIUnpacker(_unpacker);
    }

    void TearDown() override{
    }

    ~NanopbUnpackerTest() override{
        NanopbUnpacker_destroy(_unpacker);
    }

    NanopbUnpackerHandle _unpacker;
    IUnpackerHandle _iUnpacker;
    IByteStreamHandle _input;
};

TEST_F(NanopbUnpackerTest, unpack_tInc){

    vector<uint8_t> v_i{8, 10};

    for(uint8_t d : v_i){
        _input->writeByte(_input, d);
    }

    bool status = _iUnpacker->unpack(_iUnpacker, CF_T_INC);
    EXPECT_TRUE(status);
    uint32_t tInc = _iUnpacker->cfTInc_getInc(_iUnpacker);
    EXPECT_THAT(tInc, 10);
}

TEST_F(NanopbUnpackerTest, unpack_trigger){

    vector<uint8_t> v_i{8, 2, 29, 0, 0, 192, 63};

    for(uint8_t d : v_i){
        _input->writeByte(_input, d);
    }

    bool status = _iUnpacker->unpack(_iUnpacker, CF_TRIGGER);
    EXPECT_TRUE(status);
    CfTriggerDef trigger = _iUnpacker->cfTrigger_getTriggerConfig(_iUnpacker);
    EXPECT_THAT(trigger.edge, false);
    EXPECT_THAT(trigger.level, 1.5f);
    EXPECT_THAT(trigger.mode, 0);
    EXPECT_THAT(trigger.channelId, 2);
}

TEST_F(NanopbUnpackerTest, unpack_poll){

    vector<uint8_t> v_i{8, 100};

    for(uint8_t d : v_i){
        _input->writeByte(_input, d);
    }

    bool status = _iUnpacker->unpack(_iUnpacker, EV_POLL);
    EXPECT_TRUE(status);
    uint32_t poll = _iUnpacker->evPoll_getTimestamp(_iUnpacker);
    EXPECT_THAT(poll, 100);
}

TEST_F(NanopbUnpackerTest, unpack_address){

    vector<uint8_t> v_i{10, 5, 8, 2, 16, 239, 104, 10, 7, 8, 1, 16, 136, 39, 24, 1, 10, 2, 24, 2};

    for(uint8_t d : v_i){
        _input->writeByte(_input, d);
    }

    bool status = _iUnpacker->unpack(_iUnpacker, CF_ADDR);
    EXPECT_TRUE(status);
    uint32_t amount = _iUnpacker->cfAddress_getAmount(_iUnpacker);
    EXPECT_THAT(amount, 3);

    CfAddressDef addr = _iUnpacker->cfAddress_getChannel(_iUnpacker, 0);
    EXPECT_THAT(addr.address, 13423);
    EXPECT_THAT(addr.id, 2);
    EXPECT_THAT(addr.type, 0);

    addr = _iUnpacker->cfAddress_getChannel(_iUnpacker, 1);
    EXPECT_THAT(addr.address, 5000);
    EXPECT_THAT(addr.id, 1);
    EXPECT_THAT(addr.type, 1);

    addr = _iUnpacker->cfAddress_getChannel(_iUnpacker, 2);
    EXPECT_THAT(addr.address, 0);
    EXPECT_THAT(addr.id, 0);
    EXPECT_THAT(addr.type, 2);

}

TEST_F(NanopbUnpackerTest, unpack_running){

    vector<uint8_t> v_i{10, 2, 8, 1, 10, 2, 16, 1, 10, 4, 8, 2, 16, 1};

    for(uint8_t d : v_i){
        _input->writeByte(_input, d);
    }

    bool status = _iUnpacker->unpack(_iUnpacker, CF_RUNNING);
    EXPECT_TRUE(status);
    uint32_t amount = _iUnpacker->cfRunning_getAmount(_iUnpacker);
    EXPECT_THAT(amount, 3);

    CfRunningDef running = _iUnpacker->cfRunning_getChannel(_iUnpacker, 0);
    EXPECT_THAT(running.newState, false);
    EXPECT_THAT(running.id, 1);

    running = _iUnpacker->cfRunning_getChannel(_iUnpacker, 1);
    EXPECT_THAT(running.newState, true);
    EXPECT_THAT(running.id, 0);

    running = _iUnpacker->cfRunning_getChannel(_iUnpacker, 2);
    EXPECT_THAT(running.newState, true);
    EXPECT_THAT(running.id, 2);

}