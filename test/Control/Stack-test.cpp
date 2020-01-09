#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <Communication/FramedIOTestFunctions.h>

extern "C" {
#include <ScopeMock/ScopeMock.h>
#include <Scope/Core/ScopeTypes.h>
#include <Scope/Communication/Interfaces/FramedIO.h>
#include <Scope/Control/Controller.h>
#include <Scope/Serialisation/Serializer.h>
#include <Scope/Serialisation/JsonParser/JsonPacker.h>
}

using namespace testing;
using namespace std;

class StackTest : public ::testing::Test{
#define BUFFER_SIZE (300)
#define MAX_CHANNELS (3)
#define MAX_ANNOUNCE (3)
#define CHANNEL_SIZE 50

protected:
    StackTest()
            : Test(){
    }

    void SetUp() override{
        _input = BufferedByteStream_create(BUFFER_SIZE);
        _iinput = BufferedByteStream_getIByteStream(_input);

        _output = BufferedByteStream_create(Serializer_txCalculateBufferSize(MAX_CHANNELS, CHANNEL_SIZE, MAX_ANNOUNCE));
        _ioutput = BufferedByteStream_getIByteStream(_output);
        _frame = FramedIO_create(NULL, _iinput, _ioutput);

        _serializer = Serializer_create(MAX_CHANNELS, MAX_ANNOUNCE, _ioutput, _iinput);

        _packer = Serializer_getPacker(_serializer);

        _scopeMock = ScopeMock_create(MAX_CHANNELS);
        _controller = Controller_create(ScopeMock_getIScope(_scopeMock), _packer, Serializer_getUnpacker(_serializer), NULL);

        _transceiver = FramedIO_getTransceiver(_frame);
        _communicator = FramedIO_getCommunicator(_frame);

        _communicator->attachObserver(_communicator, Serializer_getUnpackObserver(_serializer));
        Serializer_attachControlObserver(_serializer, Controller_getCommandObserver(_controller));
        Controller_attachPackObserver(_controller, Serializer_getPackObserver(_serializer));
        Serializer_attachCommunicationObserver(_serializer, _communicator->getObserver(_communicator));
        _scopeUpdate = Controller_getCommandPackObserver(_controller);


    }

    void TearDown() override{
        BufferedByteStream_destroy(_output);
        BufferedByteStream_destroy(_input);
        FramedIO_destroy(_frame);
    }

    void runStack(){

        IRunnableHandle runRx = _communicator->getRxRunnable(_communicator);
        IRunnableHandle runTx = _communicator->getTxRunnable(_communicator);
        IRunnableHandle runPack = Serializer_getTxRunnable(_serializer);
        IRunnableHandle runUnpack = Serializer_getRxRunnable(_serializer);
        IRunnableHandle runCommand = Controller_getRxRunnable(_controller);
        IRunnableHandle runPackCommand = Controller_getTxRunnable(_controller);


        runRx->run(runRx);
        runUnpack->run(runUnpack);
        runCommand->run(runCommand);

        runPackCommand->run(runPackCommand);
        runPack->run(runPack);
        runTx->run(runTx);
    }

    vector<uint8_t> getFramedData(){
        size_t bytesPending = FramedIO_amountOfTxDataPending(_frame);
        uint8_t recData[bytesPending];
        FramedIO_getTxData(_frame, recData, bytesPending);
        vector<uint8_t> v_o;
        v_o.assign(recData, recData + bytesPending);
        return v_o;
    }

    ~StackTest() override{
    }

    FramedIOHandle _frame;
    BufferedByteStreamHandle _input;
    IByteStreamHandle _iinput;

    IByteStreamHandle _ioutput;
    BufferedByteStreamHandle _output;

    SerializerHandle _serializer;
    ControllerHandle _controller;
    IPackerHandle _packer;
    ITransceiverHandle _transceiver;
    ICommunicatorHandle _communicator;
    IObserverHandle _scopeUpdate;
    ScopeMockHandle _scopeMock;
};

TEST_F(StackTest, command_ev_detect){

    auto data = prepareMessage(EV_DETECT, "");

    FramedIO_putRxData(_frame, &data[0], data.size());
    runStack();

    auto recData = getFramedData();

    EXPECT_THAT(recData, ElementsAre('[', SC_DETECT, 0, 0, 0, 0, 0,0, ']'));
}

TEST_F(StackTest, command_cf_addr){

    auto command = R"({"cf_addr":{"1":[111,"SE_UINT8"],"2":[222,"SE_UINT16"],"0":[333,"SE_FLOAT"]}})";
    auto data = prepareMessage(CF_ADDR, command);

    FramedIO_putRxData(_frame, &data[0], data.size());
    runStack();

    EXPECT_TRUE(_scopeMock->configureChannelAddressHasBeenCalled);

    auto recData = getFramedData();

    EXPECT_THAT(recData, ElementsAre('[', SE_ACK, 0, 0, 0, 0, 0,0, ']'));
}

TEST_F(StackTest, command_cf_running){

    auto command = R"({"cf_running":{"1":"false","2":"true"}})";
    auto data = prepareMessage(CF_RUNNING, command);

    FramedIO_putRxData(_frame, &data[0], data.size());
    runStack();

    EXPECT_TRUE(_scopeMock->setChannelRunningHasBeenCalled);
    EXPECT_TRUE(_scopeMock->setChannelStoppedHasBeenCalled);

    auto recData = getFramedData();

    EXPECT_THAT(recData, ElementsAre('[', SE_ACK, 0, 0, 0, 0, 0,0, ']'));
}

TEST_F(StackTest, command_cf_tgr){

    auto command = R"({"cf_tgr":{"mode":"Continous","cl_id":1,"level":1.75,"edge":"falling"}})";
    auto data = prepareMessage(CF_TRIGGER, command);

    FramedIO_putRxData(_frame, &data[0], data.size());
    runStack();

    EXPECT_TRUE(_scopeMock->configureTriggerHasBeenCalled);

    auto recData = getFramedData();

    EXPECT_THAT(recData, ElementsAre('[', SE_ACK, 0, 0, 0, 0, 0,0, ']'));
}