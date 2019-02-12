#include <gtest/gtest.h>
#include <gmock/gmock.h>

extern "C" {
#include <Scope/GeneralPurpose/FloatRingBuffer.h>
#include <Scope/GeneralPurpose/BufferedFloatStream.h>
}

using namespace std;

TEST(FloatRingBuffer, test_write_read){

    const size_t shortCapacity = 10;

    const size_t shortVectorLength = 6;
    float shortTestVector[shortVectorLength] = {1.1f, 2.2f, 3.3f, 4.4f, 5.5f, 6.6f};
    float shortAnswer[shortVectorLength];

    FloatRingBufferHandle buffer = FloatRingBuffer_create(shortCapacity);

    ssize_t responce = FloatRingBuffer_write(buffer, shortTestVector, 6);
    ASSERT_EQ(responce, 6);
    responce = FloatRingBuffer_read(buffer, shortAnswer, 6);
    ASSERT_EQ(responce, 6);
    ASSERT_THAT(shortAnswer, testing::ElementsAre(1.1f, 2.2f, 3.3f, 4.4f, 5.5f, 6.6f));
}


TEST(FloatRingBuffer, test_helper_functions){
    const size_t shortCapacity = 10;

    const size_t shortVectorLength = 6;
    float shortTestVector[shortVectorLength] = {1.1f, 2.2f, 3.3f, 4.4f, 5.5f, 6.6f};
    float shortAnswer[shortVectorLength];

    FloatRingBufferHandle buffer = FloatRingBuffer_create(shortCapacity);
    size_t getCapacity = FloatRingBuffer_getCapacity(buffer);
    EXPECT_EQ(getCapacity, shortCapacity);

    ssize_t responce = FloatRingBuffer_write(buffer, shortTestVector, 6);
    EXPECT_EQ(FloatRingBuffer_getNumberOfUsedData(buffer), 6);
    EXPECT_EQ(FloatRingBuffer_getNumberOfFreeData(buffer), 4);

    FloatRingBuffer_clear(buffer);

    EXPECT_EQ(FloatRingBuffer_getNumberOfUsedData(buffer), 0);
    EXPECT_EQ(FloatRingBuffer_getNumberOfFreeData(buffer), 10);
}


TEST(FloatRingBuffer, test_exceptions){

    const size_t shortCapacity = 10;

    const size_t shortVectorLength = 6;
    float shortTestVector[shortVectorLength] = {1.1f, 2.2f, 3.3f, 4.4f, 5.5f, 6.6f};
    float shortAnswer[shortVectorLength];

    const size_t longVectorLength = 11;
    float longTestVector[longVectorLength] = {1.1f, 2.2f, 3.3f, 4.4f, 5.5f, 6.6f, 7.7f, 8.8f, 9.9f, 10.10f, 11.11f};
    float longAnswer[longVectorLength];

    FloatRingBufferHandle buffer = FloatRingBuffer_create(shortCapacity);
    ssize_t responce = FloatRingBuffer_write(buffer, longTestVector, longVectorLength);

    /* First test edge cases for writing */
    ASSERT_EQ(responce, -1);
    FloatRingBuffer_clear(buffer);
    responce = FloatRingBuffer_write(buffer, shortTestVector, shortVectorLength);
    responce = FloatRingBuffer_write(buffer, shortTestVector, shortVectorLength);
    ASSERT_EQ(responce, -1);

    /* Test edge cases for reading */
    responce = FloatRingBuffer_read(buffer, longAnswer, longVectorLength);
    ASSERT_EQ(responce, -1);
    responce = FloatRingBuffer_read(buffer, shortAnswer, 6);
    /* First read should yield proper results */
    ASSERT_THAT(shortAnswer, testing::ElementsAre(1.1f, 2.2f, 3.3f, 4.4f, 5.5f, 6.6f));
    /* Second read should yield a since less used data is aviable than requested */
    responce = FloatRingBuffer_read(buffer, shortAnswer, 6);
    ASSERT_EQ(responce, -1);
}