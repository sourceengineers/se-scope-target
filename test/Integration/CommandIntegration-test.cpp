#include <gtest/gtest.h>
#include <string>
#include <gmock/gmock-matchers.h>

extern "C" {
#include <Scope/Core/Scope.h>
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

class CommandIntegrationTest : public ::testing::Test{
#define MAX_NUMBER_OF_CHANNELS (3)
#define SIZE_OF_CHANNELS (50)
#define ADDRESSES_TO_ANNOUNCE (5)
#define TIMEBASE 1.5f

protected:
    CommandIntegrationTest()
            : Test(){
    }

    void SetUp() override{

        /* Let the Packer and Unpacker calculate how much buffer space they are going to use */
        _outputSize = Serializer_txCalculateBufferSize(MAX_NUMBER_OF_CHANNELS, SIZE_OF_CHANNELS, ADDRESSES_TO_ANNOUNCE);
        _inputSize = Serializer_rxCalculateBufferSize(ADDRESSES_TO_ANNOUNCE);

        /* Generate the input and output buffers, based on the previously calculated sizes */
        _input = BufferedByteStream_create(_inputSize);
        _output = BufferedByteStream_create(_outputSize);

        /* Generate the address storage. The address storage is optional and doesn't have to be used */
        _announcement = AnnounceStorage_create(ADDRESSES_TO_ANNOUNCE,
                                                       MAX_NUMBER_OF_CHANNELS, TIMEBASE);

        _scope = Scope_create(SIZE_OF_CHANNELS, MAX_NUMBER_OF_CHANNELS, &_timestamp);

        _ioutput = BufferedByteStream_getIByteStream(_output);
        _iinput = BufferedByteStream_getIByteStream(_input);

        _serializer = Serializer_create(MAX_NUMBER_OF_CHANNELS, ADDRESSES_TO_ANNOUNCE, _ioutput, _iinput);
        _controller = Controller_create(Scope_getIScope(_scope), Serializer_getPacker(_serializer),
                                             Serializer_getUnpacker(_serializer), _announcement);

        _oObserver = ObserverMock_create();
        /* Connect all observers */
        _iObserver = Serializer_getUnpackObserver(_serializer);
        Serializer_attachControlObserver(_serializer, Controller_getCommandObserver(_controller));
        Scope_attachPackObserver(_scope, Controller_getCommandPackObserver(_controller));
        Controller_attachPackObserver(_controller, Serializer_getPackObserver(_serializer));
        Serializer_attachCommunicationObserver(_serializer, ObserverMock_getIObserver(_oObserver));

        _runnable.scope = Scope_getIScope(_scope);
        _runnable.controller = _controller;
        _runnable.runScope = Scope_getIRunnable(_scope);
        _runnable.runCommandParser = Controller_getRxRunnable(_controller);
        _runnable.runDataAggregator = Controller_getTxRunnable(_controller);
        _runnable.runUnpacker = Serializer_getRxRunnable(_serializer);
        _runnable.runPacker = Serializer_getTxRunnable(_serializer);
    }

    void runStack(){
        _runnable.runUnpacker->run(_runnable.runUnpacker);
        _runnable.runCommandParser->run(_runnable.runCommandParser);
        _runnable.runScope->run(_runnable.runScope);
        _runnable.runDataAggregator->run(_runnable.runDataAggregator);
        _runnable.runPacker->run(_runnable.runPacker);
    }

    void ackEvent() {
        // Ack the Detect
        MessageType msg = SE_ACK;
        _iObserver->update(_iObserver, &msg);
        runStack();
    }

    void TearDown() override{
    }

    ~CommandIntegrationTest() override{
    }

    BufferedByteStreamHandle _output;
    BufferedByteStreamHandle _input;

    IByteStreamHandle _ioutput;
    IByteStreamHandle _iinput;
    size_t _outputSize;
    size_t _inputSize;
    AnnounceStorageHandle _announcement;
    ScopeHandle _scope;
    SerializerHandle _serializer;
    ControllerHandle _controller;
    uint32_t _timestamp;
    ObserverMockHandle _oObserver;
    IObserverHandle _iObserver;
    ScopeRunnable _runnable;
};


TEST_F(CommandIntegrationTest, normal_run
){

    MessageType messageType;

    messageType = EV_DETECT;
    _iObserver->update(_iObserver, &messageType);
    runStack();

    EXPECT_TRUE(_oObserver->updateHasBeenCalled);
    EXPECT_THAT(_oObserver->updateCalledWidth, SC_DETECT);
    _oObserver->updateHasBeenCalled = false;
    _oObserver->updateCalledWidth = SE_NONE;

    // Send an announce event
    messageType = EV_ANNOUNCE;
    _iObserver->update(_iObserver, &messageType);
    runStack();

    // Expect false since the SC_DETECT event has to be acked first
    EXPECT_FALSE(_oObserver->updateHasBeenCalled);

    ackEvent();

    // Now the announce should automatically be sent as it has been buffered
    EXPECT_TRUE(_oObserver->updateHasBeenCalled);
    EXPECT_THAT(_oObserver->updateCalledWidth, SC_ANNOUNCE);
    _oObserver->updateHasBeenCalled = false;
    _oObserver->updateCalledWidth = SE_NONE;
    _ioutput->flush(_ioutput);

    ackEvent();

    vector<uint8_t> v_i = {10, 2, 8, 1, 10, 0};
    _iinput->write(_iinput, &v_i[0], v_i.size());
    messageType = CF_RUNNING;
    _iObserver->update(_iObserver, &messageType);
    runStack();
    EXPECT_TRUE(_oObserver->updateHasBeenCalled);
    EXPECT_THAT(_oObserver->updateCalledWidth, SE_ACK);
    v_i.clear();
    _oObserver->updateHasBeenCalled = false;
    _oObserver->updateCalledWidth = SE_NONE;
    _ioutput->flush(_ioutput);

    v_i = {10, 5, 8, 2, 16, 239, 104, 10, 7, 8, 1, 16, 136, 39, 24, 1, 10, 2, 24, 2};
    _iinput->write(_iinput, &v_i[0], v_i.size());
    messageType = CF_ADDR;
    _iObserver->update(_iObserver, &messageType);
    runStack();
    EXPECT_TRUE(_oObserver->updateHasBeenCalled);
    EXPECT_THAT(_oObserver->updateCalledWidth, SE_ACK);
    v_i.clear();
    _oObserver->updateHasBeenCalled = false;
    _oObserver->updateCalledWidth = SE_NONE;
    _ioutput->flush(_ioutput);
}

TEST_F(CommandIntegrationTest, tx_event_buffering
){

    MessageType messageType;

    messageType = EV_DETECT;
    _iObserver->update(_iObserver, &messageType);
    runStack();
    EXPECT_TRUE(_oObserver->updateHasBeenCalled);
    EXPECT_THAT(_oObserver->updateCalledWidth, SC_DETECT);
    _oObserver->updateHasBeenCalled = false;
    _oObserver->updateCalledWidth = SE_NONE;

    // The control layer is now waiting for an ack which allows us to test the buffering by filling the buffer up
    messageType = EV_DETECT;
    _iObserver->update(_iObserver, &messageType);
    runStack();
    EXPECT_FALSE(_oObserver->updateHasBeenCalled);
    messageType = EV_TRANS;
    _iObserver->update(_iObserver, &messageType);
    runStack();
    EXPECT_FALSE(_oObserver->updateHasBeenCalled);
    messageType = EV_ANNOUNCE;
    _iObserver->update(_iObserver, &messageType);
    runStack();
    EXPECT_FALSE(_oObserver->updateHasBeenCalled);

    // Now we are staring to ack events
    ackEvent();
    EXPECT_TRUE(_oObserver->updateHasBeenCalled);
    EXPECT_THAT(_oObserver->updateCalledWidth, SC_DETECT);
    _oObserver->updateHasBeenCalled = false;
    _oObserver->updateCalledWidth = SE_NONE;

    // Now we are staring to ack events
    ackEvent();
    EXPECT_TRUE(_oObserver->updateHasBeenCalled);
    EXPECT_THAT(_oObserver->updateCalledWidth, SC_DATA);
    _oObserver->updateHasBeenCalled = false;
    _oObserver->updateCalledWidth = SE_NONE;

    // Now we are staring to ack events
    ackEvent();
    EXPECT_TRUE(_oObserver->updateHasBeenCalled);
    EXPECT_THAT(_oObserver->updateCalledWidth, SC_ANNOUNCE);
    _oObserver->updateHasBeenCalled = false;
    _oObserver->updateCalledWidth = SE_NONE;


}

TEST_F(CommandIntegrationTest, tx_event_buffering_overflow
){
    MessageType messageType;
    // The buffer is 7 elements big, so with 10 events, we can test the buffer overflow of the event buffer
    for(int i = 0; i < 10; ++i){
        messageType = EV_DETECT;
        _iObserver->update(_iObserver, &messageType);
        runStack();
        messageType = SE_ACK;
        _iObserver->update(_iObserver, &messageType);
        runStack();
        EXPECT_TRUE(_oObserver->updateHasBeenCalled);
        EXPECT_THAT(_oObserver->updateCalledWidth, SC_DETECT);
        _oObserver->updateHasBeenCalled = false;
        _oObserver->updateCalledWidth = SE_NONE;
    }
}