#include <gtest/gtest.h>
#include <gmock/gmock.h>

extern "C" {
#include <Scope/Core/Channel.h>
}

using namespace std;

TEST(Channel, test_polling){
    const size_t shortCapacity = 10;

    const size_t shortVectorLength = 6;
    float shortTestVector[shortVectorLength] = {1.1f, 2.2f, 3.3f, 4.4f, 5.5f, 6.6f};
    float shortAnswer[shortVectorLength];

    float data;

    /* Create Instanzes */
    ChannelHandle channel = Channel_create(shortCapacity);

    /* Configure channel */
    Channel_setPollAddress(channel, &data, FLOAT);
    Channel_setStateRunning(channel);

    /* Simulate polling */
    for(size_t i = 0; i < shortVectorLength; i++){
        data = shortTestVector[i];
        Channel_poll(channel);
    }
    Channel_swapBuffers(channel);

    Channel_read(channel, shortAnswer, shortVectorLength);

    ASSERT_THAT(shortAnswer, testing::ElementsAre(1.1f, 2.2f, 3.3f, 4.4f, 5.5f, 6.6f));
}


TEST(Channel, test_data_types){
    const size_t shortCapacity = 10;

    const size_t shortVectorLength = 6;
    float shortAnswer[shortVectorLength];

    /* Create Instanzes */
    ChannelHandle channel = Channel_create(shortCapacity);
    float answer;

    float data = 5.5f;
    /* Configure channel */
    Channel_setPollAddress(channel, &data, FLOAT);
    Channel_setStateRunning(channel);
    Channel_poll(channel);
    Channel_swapBuffers(channel);
    Channel_read(channel, &answer, 1);
    EXPECT_EQ(answer, 5.5f);

    uint8_t uint8Data = 12;
    Channel_setPollAddress(channel, &uint8Data, UINT8);
    Channel_poll(channel);
    Channel_swapBuffers(channel);
    Channel_read(channel, &answer, 1);
    EXPECT_EQ((uint8_t) answer, 12);

    uint16_t uint16Data = 12;
    Channel_setPollAddress(channel, &uint16Data, UINT16);
    Channel_poll(channel);
    Channel_swapBuffers(channel);
    Channel_read(channel, &answer, 1);
    EXPECT_EQ((uint16_t) answer, 12);

    uint32_t uint32Data = 12;
    Channel_setPollAddress(channel, &uint32Data, UINT32);
    Channel_poll(channel);
    Channel_swapBuffers(channel);
    Channel_read(channel, &answer, 1);
    EXPECT_EQ((uint32_t) answer, 12);

}

TEST(Channel, test_states){
    const size_t shortCapacity = 10;

    const size_t shortVectorLength = 6;
    float shortTestVector[shortVectorLength] = {1.1f, 2.2f, 3.3f, 4.4f, 5.5f, 6.6f};
    float shortAnswer[shortVectorLength];

    float data;

    /* Create Instanzes */
    ChannelHandle channel = Channel_create(shortCapacity);

    bool isRunning = Channel_setStateRunning(channel);
    EXPECT_EQ(isRunning, false);
    bool isStopped = Channel_setStateStopped(channel);
    EXPECT_EQ(isStopped, false);

    /* Configure channel */
    Channel_setPollAddress(channel, &data, FLOAT);
    isRunning = Channel_setStateRunning(channel);
    EXPECT_EQ(isRunning, true);
    data = 5.5f;
    isStopped = Channel_setStateStopped(channel);
    EXPECT_EQ(isStopped, true);
}

TEST(Channel, test_trigger_stream){
    const size_t shortCapacity = 10;

    const size_t shortVectorLength = 6;
    float shortTestVector[shortVectorLength] = {1.1f, 2.2f, 3.3f, 4.4f, 5.5f, 6.6f};

    float data;

    /* Create Instanzes */
    ChannelHandle channel = Channel_create(shortCapacity);

    /* Configure channel */
    Channel_setPollAddress(channel, &data, FLOAT);
    Channel_setStateRunning(channel);

    /* Simulate polling */
    for(size_t i = 0; i < shortVectorLength; i++){
        data = shortTestVector[i];
        Channel_poll(channel);
    }

    /* Get stream interface to test the procedure */
    IFloatStreamHandle stream = Channel_getTriggerDataStream(channel);

    size_t readData;
    readData = stream->length(stream);
    ASSERT_EQ(readData, 2);

    float triggerData[2];
    stream->read(stream, triggerData, 2);

    ASSERT_THAT(triggerData, testing::ElementsAre(5.5f, 6.6f));

}
