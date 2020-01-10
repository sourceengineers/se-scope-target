#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <Communication/FramedIOTestFunctions.h>

extern "C" {
#include "Scope/Control/AnnounceStorage.h"
}

using namespace testing;
using namespace std;

class AnnounceTest : public ::testing::Test{
#define BUFFER_SIZE (300)
#define MAX_CHANNELS (3)
#define MAX_ANNOUNCE (3)
#define CHANNEL_SIZE 50

protected:
    AnnounceTest()
            : Test(){
    }

    void SetUp() override{
        _announce = AnnounceStorage_create(4, 9, 0.000001f);
    }

    void TearDown() override{
        AnnounceStorage_destroy(_announce);
    }


    ~AnnounceTest() override{
    }

    AnnounceStorageHandle _announce;
};



TEST_F(AnnounceTest, check_increment_configured_addresses){
    EXPECT_EQ(4, AnnounceStorage_getMaxAmountOfAddresses(_announce));
    EXPECT_EQ(9, AnnounceStorage_getMaxAmountOfChannels(_announce));
    EXPECT_EQ(0.000001f, AnnounceStorage_getTimeBase(_announce));

    float testSignal0 = 0.0f;
    float testSignal1 = 0.0f;
    float testSignal2 = 0.0f;
    float testSignal3 = 0.0f;
    float testSignal4 = 0.0f;

    EXPECT_EQ(0, AnnounceStorage_getAmountOfConfiguredAddresses(_announce));
    EXPECT_TRUE(AnnounceStorage_addAnnounceAddress(_announce, "testSignal0", &testSignal0, SE_FLOAT));
    EXPECT_EQ(1, AnnounceStorage_getAmountOfConfiguredAddresses(_announce));
    EXPECT_TRUE(AnnounceStorage_addAnnounceAddress(_announce, "testSignal1", &testSignal1, SE_FLOAT));
    EXPECT_EQ(2, AnnounceStorage_getAmountOfConfiguredAddresses(_announce));
    EXPECT_TRUE(AnnounceStorage_addAnnounceAddress(_announce, "testSignal2", &testSignal2, SE_FLOAT));
    EXPECT_EQ(3, AnnounceStorage_getAmountOfConfiguredAddresses(_announce));
    EXPECT_TRUE(AnnounceStorage_addAnnounceAddress(_announce, "testSignal3", &testSignal3, SE_FLOAT));
    EXPECT_EQ(4, AnnounceStorage_getAmountOfConfiguredAddresses(_announce));
    EXPECT_FALSE(AnnounceStorage_addAnnounceAddress(_announce, "testSignal4", &testSignal4, SE_FLOAT));
    EXPECT_EQ(4, AnnounceStorage_getAmountOfConfiguredAddresses(_announce));


    EXPECT_EQ(AnnounceStorage_getAddressToTransmit(_announce, 0)->type, SE_FLOAT);
    EXPECT_GT(AnnounceStorage_getAddressToTransmit(_announce, 0)->address, 0);
    EXPECT_STREQ(AnnounceStorage_getAddressToTransmit(_announce, 0)->name, "testSignal0");

    EXPECT_EQ(AnnounceStorage_getAddressToTransmit(_announce, 1)->type, SE_FLOAT);
    EXPECT_GT(AnnounceStorage_getAddressToTransmit(_announce, 1)->address, 0);
    EXPECT_STREQ(AnnounceStorage_getAddressToTransmit(_announce, 1)->name, "testSignal1");

    EXPECT_EQ(AnnounceStorage_getAddressToTransmit(_announce, 2)->type, SE_FLOAT);
    EXPECT_GT(AnnounceStorage_getAddressToTransmit(_announce, 2)->address, 0);
    EXPECT_STREQ(AnnounceStorage_getAddressToTransmit(_announce, 2)->name, "testSignal2");

    EXPECT_EQ(AnnounceStorage_getAddressToTransmit(_announce, 3)->type, SE_FLOAT);
    EXPECT_GT(AnnounceStorage_getAddressToTransmit(_announce, 3)->address, 0);
    EXPECT_STREQ(AnnounceStorage_getAddressToTransmit(_announce, 3)->name, "testSignal3");

}

TEST_F(AnnounceTest, check_one_one){
    EXPECT_EQ(1, 1);
}
