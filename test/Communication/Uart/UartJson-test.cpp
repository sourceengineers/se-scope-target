#include <gtest/gtest.h>
#include <Scope/Core/ScopeTypes.h>
#include <gmock/gmock-matchers.h>
#include <stdbool.h>

extern "C" {
#include <Scope/Communication/Interfaces/UartJson.h>
#include <Scope/GeneralPurpose/BufferedByteStream.h>
#include "../../Observer/ObserverMock.h"
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
        _observer = ObserverMock_create();
        ICommunicatorHandle comm = UartJson_getCommunicator(_uart);
        comm->attachObserver(comm, ObserverMock_getIObserver(_observer));
    }

    void runTx(MessageType type){
        ICommunicatorHandle comm = UartJson_getCommunicator(_uart);
        IObserverHandle obs = comm->getObserver(comm);
        obs->update(obs, &type);
        IRunnableHandle runTx = comm->getTxRunnable(comm);
        runTx->run(runTx);
    }

    void runRx(){
        ICommunicatorHandle comm = UartJson_getCommunicator(_uart);
        IRunnableHandle runRx = comm->getRxRunnable(comm);
        runRx->run(runRx);
    }

    void TearDown() override{
        BufferedByteStream_destroy(_output);
        BufferedByteStream_destroy(_input);
        UartJson_destroy(_uart);
    }

    void writeLength(uint32_t length, vector<uint8_t> &container){
        for(size_t i = 0; i < 4; ++i){
            uint8_t byte = (length >> (3 - i) * 4) & 0xF;
            container.push_back(byte);
        }
    }

    void writeChecksum(uint8_t* data, size_t length, vector<uint8_t> &container){

        uint16_t checksum = 0;
        for(size_t i = 0; i < length; ++i){
            uint8_t byte = data[i];
            checksum += byte;
        }

        container.push_back((checksum & 0xF0) >> 4);
        container.push_back(checksum & 0xF);
    }


    ~UartTest() override{
    }

    UartJsonHandle _uart;
    BufferedByteStreamHandle _input;
    IByteStreamHandle _iinput;

    IByteStreamHandle _ioutput;
    BufferedByteStreamHandle _output;
    ObserverMockHandle _observer;
};

/**
 * Feed data as if they would be perfectly received
 */
TEST_F(UartTest, input_accept){

    size_t dataLength = 5;

    uint8_t data[] = {1, 2, 3, 4, 5};

    vector<uint8_t> v_i;
    v_i.push_back('[');
    v_i.push_back(EV_ANNOUNCE);
    writeLength(5, v_i);

    for(int i = 0; i < dataLength; ++i){
        v_i.push_back(data[i]);
    }

    writeChecksum(data, dataLength, v_i);
    v_i.push_back(']');

    bool parsingValid = UartJson_putRxData(_uart, &v_i[0], v_i.size());
    EXPECT_TRUE(parsingValid);
    runRx();

    EXPECT_TRUE(_observer->updateHasBeenCalled);
    EXPECT_EQ(_observer->updateCalledWidth, 100);

    size_t length = _iinput->length(_iinput);
    EXPECT_EQ(length, 5);

    uint8_t readData[length];
    _iinput->read(_iinput, readData, length);
    vector<uint8_t> v_o;
    v_o.assign(readData, readData + length);
    EXPECT_THAT(v_o, ElementsAre(1, 2, 3, 4, 5));
}

/**
 * Feed data as if they would be perfectly received
 */
TEST_F(UartTest, input_accept_feed_as_chuncks){

    size_t dataLength = 5;

    uint8_t data[] = {1, 2, 3, 4, 5};

    vector<uint8_t> v_i;
    v_i.push_back('[');
    v_i.push_back(EV_ANNOUNCE);
    writeLength(5, v_i);

    for(int i = 0; i < dataLength; ++i){
        v_i.push_back(data[i]);
    }

    writeChecksum(data, dataLength, v_i);
    v_i.push_back(']');

    bool parsed = UartJson_putRxData(_uart, &v_i[0], 3);
    EXPECT_FALSE(parsed);
    runRx();

    parsed = UartJson_putRxData(_uart, &v_i[3], 6);
    EXPECT_FALSE(parsed);
    runRx();

    parsed = UartJson_putRxData(_uart, &v_i[9], 2);
    EXPECT_FALSE(parsed);
    runRx();

    parsed = UartJson_putRxData(_uart, &v_i[11], 5);
    EXPECT_TRUE(parsed);
    runRx();

    EXPECT_TRUE(_observer->updateHasBeenCalled);
    EXPECT_EQ(_observer->updateCalledWidth, 100);

    size_t length = _iinput->length(_iinput);
    EXPECT_EQ(length, 5);

    uint8_t readData[length];
    _iinput->read(_iinput, readData, length);
    vector<uint8_t> v_o;
    v_o.assign(readData, readData + length);
    EXPECT_THAT(v_o, ElementsAre(1, 2, 3, 4, 5));
}

TEST_F(UartTest, input_accept_feed_full_and_partial){

    size_t dataLength = 5;

    uint8_t data[] = {1, 2, 3, 4, 5};

    vector<uint8_t> v_i;
    v_i.push_back('[');
    v_i.push_back(EV_ANNOUNCE);
    writeLength(5, v_i);

    for(int i = 0; i < dataLength; ++i){
        v_i.push_back(data[i]);
    }

    writeChecksum(data, dataLength, v_i);
    v_i.push_back(']');

    v_i.push_back('[');
    v_i.push_back(EV_ANNOUNCE);
    writeLength(5, v_i);

    bool valid = UartJson_putRxData(_uart, &v_i[0], v_i.size());
    EXPECT_TRUE(valid);
    runRx();
    EXPECT_TRUE(_observer->updateHasBeenCalled);
    EXPECT_EQ(_observer->updateCalledWidth, 100);

    size_t length = _iinput->length(_iinput);
    EXPECT_EQ(length, 5);

    uint8_t readData[length];
    _iinput->read(_iinput, readData, length);
    vector<uint8_t> v_o;
    v_o.assign(readData, readData + length);
    EXPECT_THAT(v_o, ElementsAre(1, 2, 3, 4, 5));
}

TEST_F(UartTest, input_accept_feed_multiple){

    size_t dataLength = 5;

    uint8_t data[] = {1, 2, 3, 4, 5};

    vector<uint8_t> v_i;
    v_i.push_back('[');
    v_i.push_back(EV_ANNOUNCE);
    writeLength(5, v_i);

    for(int i = 0; i < dataLength; ++i){
        v_i.push_back(data[i]);
    }

    writeChecksum(data, dataLength, v_i);
    v_i.push_back(']');

    bool valid = UartJson_putRxData(_uart, &v_i[0], v_i.size());
    EXPECT_EQ(true, valid);
    runRx();
    EXPECT_TRUE(_observer->updateHasBeenCalled);
    EXPECT_EQ(_observer->updateCalledWidth, 100);

    size_t length = _iinput->length(_iinput);
    EXPECT_EQ(length, 5);

    uint8_t readData[length];
    _iinput->read(_iinput, readData, length);
    vector<uint8_t> v_o;
    v_o.assign(readData, readData + length);
    EXPECT_THAT(v_o, ElementsAre(1, 2, 3, 4, 5));

    valid = UartJson_putRxData(_uart, &v_i[0], v_i.size());
    EXPECT_EQ(true, valid);
    runRx();
    EXPECT_TRUE(_observer->updateHasBeenCalled);
    EXPECT_EQ(_observer->updateCalledWidth, 100);

    length = _iinput->length(_iinput);
    EXPECT_EQ(length, 5);

    _iinput->read(_iinput, readData, length);
    v_o.assign(readData, readData + length);
    EXPECT_THAT(v_o, ElementsAre(1, 2, 3, 4, 5));

    valid = UartJson_putRxData(_uart, &v_i[0], v_i.size());
    EXPECT_EQ(true, valid);
    runRx();
    EXPECT_TRUE(_observer->updateHasBeenCalled);
    EXPECT_EQ(_observer->updateCalledWidth, 100);

    length = _iinput->length(_iinput);
    EXPECT_EQ(length, 5);

    _iinput->read(_iinput, readData, length);
    v_o.assign(readData, readData + length);
    EXPECT_THAT(v_o, ElementsAre(1, 2, 3, 4, 5));
}

/**
 * Feed data as if they would be perfectly received
 */
TEST_F(UartTest, input_accept_feed_garbage){

    size_t dataLength = 5;

    uint8_t data[] = {1, 2, 3, 4, 5};

    // Write wrong length in data
    vector<uint8_t> v_i;
    v_i.push_back('[');
    v_i.push_back(EV_ANNOUNCE);
    writeLength(20, v_i); // not correct size
    for(int i = 0; i < dataLength; ++i){
        v_i.push_back(data[i]);
    }
    writeChecksum(data, dataLength, v_i);
    v_i.push_back(']');
    bool parsed = UartJson_putRxData(_uart, &v_i[0], v_i.size());
    EXPECT_FALSE(parsed);
    runRx();
    EXPECT_FALSE(_observer->updateHasBeenCalled);

    // Write wrong checksum
    v_i.clear();
    v_i.push_back('[');
    v_i.push_back(EV_ANNOUNCE);
    writeLength(5, v_i);
    for(int i = 0; i < dataLength; ++i){
        v_i.push_back(data[i]);
    }
    writeChecksum(data, dataLength - 2, v_i);      // Use wrong length to produce checksum, chause a faulty sum
    v_i.push_back(']');
    parsed = UartJson_putRxData(_uart, &v_i[0], v_i.size());
    EXPECT_FALSE(parsed);
    runRx();
    EXPECT_FALSE(_observer->updateHasBeenCalled);

    // Wrong data length
    v_i.clear();
    v_i.push_back('[');
    v_i.push_back(EV_ANNOUNCE);
    writeLength(5, v_i);
    for(int i = 0; i < dataLength - 2; ++i){ // Don't write enough data in the input
        v_i.push_back(data[i]);
    }
    writeChecksum(data, dataLength, v_i);
    v_i.push_back(']');
    parsed = UartJson_putRxData(_uart, &v_i[0], v_i.size());
    EXPECT_FALSE(parsed);
    runRx();
    EXPECT_FALSE(_observer->updateHasBeenCalled);

    // No stop flag
    v_i.clear();
    v_i.push_back('[');
    v_i.push_back(EV_ANNOUNCE);
    writeLength(5, v_i);
    for(int i = 0; i < dataLength - 2; ++i){
        v_i.push_back(data[i]);
    }
    writeChecksum(data, dataLength, v_i);
    parsed = UartJson_putRxData(_uart, &v_i[0], v_i.size());
    EXPECT_FALSE(parsed);
    runRx();
    EXPECT_FALSE(_observer->updateHasBeenCalled);

    /**
     * Normal test case as before. The protocol has to be able to recover after receiving a bunch of faulty data
     */
    v_i.clear();
    v_i.push_back('[');
    v_i.push_back(EV_ANNOUNCE);
    writeLength(5, v_i);

    for(int i = 0; i < dataLength; ++i){
        v_i.push_back(data[i]);
    }

    writeChecksum(data, dataLength, v_i);
    v_i.push_back(']');
    parsed = UartJson_putRxData(_uart, &v_i[0], v_i.size());
    EXPECT_TRUE(parsed);
    runRx();
    EXPECT_TRUE(_observer->updateHasBeenCalled);
    EXPECT_EQ(_observer->updateCalledWidth, 100);
    size_t length = _iinput->length(_iinput);
    EXPECT_EQ(length, 5);

    uint8_t readData[length];
    _iinput->read(_iinput, readData, length);
    vector<uint8_t> v_o;
    v_o.assign(readData, readData + length);
    EXPECT_THAT(v_o, ElementsAre(1, 2, 3, 4, 5));

}


TEST_F(UartTest, output_normal){

    for(uint8_t i = 0; i < 6; ++i){
        _ioutput->writeByte(_ioutput, i);
    }
    runTx(SC_ANNOUNCE);
    size_t dataPending = UartJson_amountOfTxDataPending(_uart);
    EXPECT_EQ(dataPending, 15);
    vector<uint8_t> v_o;
    uint8_t readData[dataPending];
    UartJson_getTxData(_uart, readData, dataPending);
    v_o.assign(readData, readData + dataPending);
    EXPECT_THAT(v_o, ElementsAre('[', 51, 0, 0, 0, 6, 0, 1, 2, 3, 4, 5, 0, 15, ']'));
}

TEST_F(UartTest, output_read_chunks){

    const size_t chunkSize = 4;
    for(uint8_t i = 0; i < 6; ++i){
        _ioutput->writeByte(_ioutput, i);
    }
    runTx(SC_ANNOUNCE);
    vector<uint8_t> v_o;
    uint8_t readData[chunkSize];
    EXPECT_EQ(UartJson_amountOfTxDataPending(_uart), 15);
    UartJson_getTxData(_uart, readData, chunkSize);
    v_o.assign(readData, readData + chunkSize);
    EXPECT_THAT(v_o, ElementsAre('[', 51, 0, 0));
    v_o.clear();
    EXPECT_EQ(UartJson_amountOfTxDataPending(_uart), 11);
    UartJson_getTxData(_uart, readData, chunkSize);
    v_o.assign(readData, readData + chunkSize);
    EXPECT_THAT(v_o,ElementsAre(0, 6, 0, 1));
    v_o.clear();
    EXPECT_EQ(UartJson_amountOfTxDataPending(_uart), 7);
    UartJson_getTxData(_uart, readData, chunkSize);
    v_o.assign(readData, readData + chunkSize);
    EXPECT_THAT(v_o,ElementsAre(2, 3, 4, 5));
    v_o.clear();
    EXPECT_EQ(UartJson_amountOfTxDataPending(_uart), 3);
    UartJson_getTxData(_uart, readData, chunkSize);
    v_o.assign(readData, readData + 3);
    EXPECT_THAT(v_o,ElementsAre(0, 15, ']'));
}


TEST_F(UartTest, output_multiple){

    for(uint8_t i = 0; i < 6; ++i){
        _ioutput->writeByte(_ioutput, i);
    }
    runTx(SC_ANNOUNCE);
    size_t dataPending = UartJson_amountOfTxDataPending(_uart);
    EXPECT_EQ(dataPending, 15);
    vector<uint8_t> v_o;
    uint8_t readData[dataPending];
    UartJson_getTxData(_uart, readData, dataPending);
    v_o.assign(readData, readData + dataPending);
    EXPECT_THAT(v_o, ElementsAre('[', 51, 0, 0, 0, 6, 0, 1, 2, 3, 4, 5, 0, 15, ']'));

    for(uint8_t i = 0; i < 6; ++i){
        _ioutput->writeByte(_ioutput, i);
    }
    runTx(SC_ANNOUNCE);
    dataPending = UartJson_amountOfTxDataPending(_uart);
    EXPECT_EQ(dataPending, 15);
    v_o.clear();
    UartJson_getTxData(_uart, readData, dataPending);
    v_o.assign(readData, readData + dataPending);
    EXPECT_THAT(v_o, ElementsAre('[', 51, 0, 0, 0, 6, 0, 1, 2, 3, 4, 5, 0, 15, ']'));
}

TEST_F(UartTest, output_try_overflow){

    for(uint8_t i = 0; i < 6; ++i){
        _ioutput->writeByte(_ioutput, i);
    }
    runTx(SC_ANNOUNCE);
    size_t dataPending = UartJson_amountOfTxDataPending(_uart);
    EXPECT_EQ(dataPending, 15);
    vector<uint8_t> v_o;
    uint8_t readData[dataPending];
    // don't pull all data
    UartJson_getTxData(_uart, readData, dataPending - 5);
    v_o.assign(readData, readData + (dataPending - 5));
    EXPECT_THAT(v_o, ElementsAre('[', 51, 0, 0, 0, 6, 0, 1, 2, 3));

    // Run runTx again before all data was pulled
    runTx(SC_ANNOUNCE);
    UartJson_getTxData(_uart, readData, 5);
    v_o.assign(readData, readData + 5);
    EXPECT_THAT(v_o, ElementsAre(4, 5, 0, 15, ']'));

}