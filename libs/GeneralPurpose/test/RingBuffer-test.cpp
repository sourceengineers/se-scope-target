#include <gtest/gtest.h>
#include <gmock/gmock.h>

extern "C" {
    #include "RingBuffer.h"
}

TEST(RingBuffer, test_write_read)
{
  
  const size_t shortCapacity = 10;

  const size_t shortVectorLength = 6;
  float shortTestVector[shortVectorLength] = {1.1f,2.2f,3.3f,4.4f,5.5f,6.6f};
  float shortAnswer[shortVectorLength];
  
  RingBufferHandle buffer = RingBuffer_create(shortCapacity);  
  
  ssize_t responce = RingBuffer_write(buffer, shortTestVector, 6);
  ASSERT_EQ(responce, 6);
  responce = RingBuffer_read(buffer, shortAnswer, 6);
  ASSERT_EQ(responce, 6);
  ASSERT_THAT(shortAnswer, testing::ElementsAre(1.1f,2.2f,3.3f,4.4f,5.5f,6.6f));
}


TEST(RingBuffer, test_helper_functions)
{
  const size_t shortCapacity = 10;

  const size_t shortVectorLength = 6;
  float shortTestVector[shortVectorLength] = {1.1f,2.2f,3.3f,4.4f,5.5f,6.6f};
  float shortAnswer[shortVectorLength];
  
  RingBufferHandle buffer = RingBuffer_create(shortCapacity);
  size_t getCapacity = RingBuffer_getCapacity(buffer);
  EXPECT_EQ(getCapacity, shortCapacity);
  
  ssize_t responce = RingBuffer_write(buffer, shortTestVector, 6);
  EXPECT_EQ(RingBuffer_usedData(buffer), 6);
  EXPECT_EQ(RingBuffer_freeData(buffer), 4);
  
  RingBuffer_clear(buffer);
  
  EXPECT_EQ(RingBuffer_usedData(buffer), 0);
  EXPECT_EQ(RingBuffer_freeData(buffer), 10);
}


TEST(RingBuffer, test_exceptions)
{
  
  const size_t shortCapacity = 10;

  const size_t shortVectorLength = 6;
  float shortTestVector[shortVectorLength] = {1.1f,2.2f,3.3f,4.4f,5.5f,6.6f};
  float shortAnswer[shortVectorLength];

  const size_t longVectorLength = 11;
  float longTestVector[longVectorLength] = {1.1f,2.2f,3.3f,4.4f,5.5f,6.6f,7.7f,8.8f,9.9f,10.10f, 11.11f};
  float longAnswer[longVectorLength];
  
  RingBufferHandle buffer = RingBuffer_create(shortCapacity);
  ssize_t responce = RingBuffer_write(buffer, longTestVector, longVectorLength);
  
  /* First test edge cases for writing */
  ASSERT_EQ(responce, -1);
  RingBuffer_clear(buffer);
  responce = RingBuffer_write(buffer, shortTestVector, shortVectorLength);
  responce = RingBuffer_write(buffer, shortTestVector, shortVectorLength);
  ASSERT_EQ(responce, -1);
  
  /* Test edge cases for reading */
  responce = RingBuffer_read(buffer, longAnswer, longVectorLength);
  ASSERT_EQ(responce, -1);
  responce = RingBuffer_read(buffer, shortAnswer, 6);
  /* First read should yield proper results */
  ASSERT_THAT(shortAnswer, testing::ElementsAre(1.1f,2.2f,3.3f,4.4f,5.5f,6.6f));
  /* Second read should yield a since less used data is aviable than requested */
  responce = RingBuffer_read(buffer, shortAnswer, 6);
  ASSERT_EQ(responce, -1);
}

TEST(RingBuffer, test_stream_interface)
{
  const size_t shortCapacity = 10;

  const size_t shortVectorLength = 6;
  float shortTestVector[shortVectorLength] = {1.1f,2.2f,3.3f,4.4f,5.5f,6.6f};
  float shortAnswer[shortVectorLength];

  RingBufferHandle buffer = RingBuffer_create(shortCapacity);
  ssize_t responce = RingBuffer_write(buffer, shortTestVector, shortVectorLength);
  
  IFloatStreamHandle stream = RingBuffer_getFloatStream(buffer);
  const size_t size = stream->getSize(stream);
  
  ASSERT_EQ(size, 6);
  
  float streamData[6];
  stream->open(stream, streamData);
  stream->getStream(stream);
  stream->close(stream);
  
  ASSERT_THAT(streamData, testing::ElementsAre(1.1f,2.2f,3.3f,4.4f,5.5f,6.6f));
}
