#include <gtest/gtest.h>

extern "C" {
#include <Scope/Communication/Interfaces/UartJson.h>
#include <Scope/GeneralPurpose/BufferedByteStream.h>
}

using namespace testing;
using namespace std;

class UartTest : public ::testing::Test{
#define BUFFER_SIZE (40)

protected:
    UartTest()
            : Test(){
    }

    void SetUp() override{
        _input = BufferedByteStream_create(BUFFER_SIZE);
        _iinput = BufferedByteStream_getIByteStream(_input);
        _output = BufferedByteStream_create(BUFFER_SIZE);
        _ioutput = BufferedByteStream_getIByteStream(_output);
        _uart = UartJson_create(NULL, _iinput, _ioutput);
    }

    void TearDown() override{
        BufferedByteStream_destroy(_output);
        BufferedByteStream_destroy(_input);
        UartJson_destroy(_uart);
    }

    ~UartTest() override{
    }

    UartJsonHandle _uart;
    BufferedByteStreamHandle _input;
    IByteStreamHandle _iinput;
    IByteStreamHandle _ioutput;
    BufferedByteStreamHandle _output;
};

TEST_F(UartTest, input_accept){




    ASSERT_TRUE(false);
}

TEST_F(UartTest, input_reject){
    ASSERT_TRUE(false);
}

TEST_F(UartTest, output_get){
    ASSERT_TRUE(false);
}