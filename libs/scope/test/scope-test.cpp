#include <gtest/gtest.h>
#include <gmock/gmock.h>

extern "C" {
    #include "ring_buffer.h"
    #include "channel.h"
}

TEST(RingBuffer, test_write_read)
{
  
  const size_t shortCapacity = 10;

  const size_t shortVectorLength = 6;
  float shortTestVector[shortVectorLength] = {1.1f,2.2f,3.3f,4.4f,5.5f,6.6f};
  float shortAnswer[shortVectorLength];
  
  RingBuffer* buffer = RingBuffer_create(shortCapacity);  
  
  ssize_t responce = buffer->write(buffer, shortTestVector, 6);
  ASSERT_EQ(responce, 6);
  responce = buffer->read(buffer, shortAnswer, 6);
  ASSERT_EQ(responce, 6);
  ASSERT_THAT(shortAnswer, testing::ElementsAre(1.1f,2.2f,3.3f,4.4f,5.5f,6.6f));
}


TEST(RingBuffer, test_helper_functions)
{
  const size_t shortCapacity = 10;

  const size_t shortVectorLength = 6;
  float shortTestVector[shortVectorLength] = {1.1f,2.2f,3.3f,4.4f,5.5f,6.6f};
  float shortAnswer[shortVectorLength];
  
  RingBuffer* buffer = RingBuffer_create(shortCapacity);
  size_t getCapacity = buffer->getCapacity(buffer);
  EXPECT_EQ(getCapacity, shortCapacity);
  
  ssize_t responce = buffer->write(buffer, shortTestVector, 6);
  EXPECT_EQ(buffer->usedData(buffer), 6);
  EXPECT_EQ(buffer->freeData(buffer), 4);
  
  buffer->clear(buffer);
  
  EXPECT_EQ(buffer->usedData(buffer), 0);
  EXPECT_EQ(buffer->freeData(buffer), 10);
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
  
  RingBuffer* buffer = RingBuffer_create(shortCapacity);
  ssize_t responce = buffer->write(buffer, longTestVector, longVectorLength);
  
  /* First test edge cases for writing */
  ASSERT_EQ(responce, -1);
  buffer->clear(buffer);
  responce = buffer->write(buffer, shortTestVector, shortVectorLength);
  responce = buffer->write(buffer, shortTestVector, shortVectorLength);
  ASSERT_EQ(responce, -1);
  
  /* Test edge cases for reading */
  responce = buffer->read(buffer, longAnswer, longVectorLength);
  ASSERT_EQ(responce, -1);
  responce = buffer->read(buffer, shortAnswer, 6);
  /* First read should yield proper results */
  ASSERT_THAT(shortAnswer, testing::ElementsAre(1.1f,2.2f,3.3f,4.4f,5.5f,6.6f));
  /* Second read should yield a since less used data is aviable than requested */
  responce = buffer->read(buffer, shortAnswer, 6);
  ASSERT_EQ(responce, -1);
}

TEST(RingBuffer, test_stream_interface)
{
  const size_t shortCapacity = 10;

  const size_t shortVectorLength = 6;
  float shortTestVector[shortVectorLength] = {1.1f,2.2f,3.3f,4.4f,5.5f,6.6f};
  float shortAnswer[shortVectorLength];

  RingBuffer* buffer = RingBuffer_create(shortCapacity);
  ssize_t responce = buffer->write(buffer, shortTestVector, shortVectorLength);
  
  IFloatStream* stream = buffer->getFloatStream(buffer);
  const size_t size = stream->getSize(stream);
  
  ASSERT_EQ(size, 6);
  
  float streamData[6];
  stream->open(stream, streamData);
  stream->getStream(stream);
  stream->close(stream);
  
  ASSERT_THAT(streamData, testing::ElementsAre(1.1f,2.2f,3.3f,4.4f,5.5f,6.6f));
}


TEST(Channel, test_polling)
{
  const size_t shortCapacity = 10;

  const size_t shortVectorLength = 6;
  float shortTestVector[shortVectorLength] = {1.1f,2.2f,3.3f,4.4f,5.5f,6.6f};
  float shortAnswer[shortVectorLength];

  float data;

  /* Create Instanzes */
  RingBuffer* buffer = RingBuffer_create(shortCapacity);
  Channel* channel = Channel_create(buffer);
  
  /* Configure channel */
  channel->setPollAddress(channel, &data);
  channel->setStateRunning(channel);
  
  /* Simulate polling */
  for(size_t i = 0; i < shortVectorLength; i++){
    data = shortTestVector[i];
    channel->poll(channel);
  }
  
  /* Get stream interface to test the procedure */
  IFloatStream* stream = channel->getFloatStream(channel);
  stream->open(stream, shortAnswer);
  stream->getStream(stream);
  stream->close(stream);
  ASSERT_THAT(shortAnswer, testing::ElementsAre(1.1f,2.2f,3.3f,4.4f,5.5f,6.6f));
}

TEST(Channel, test_states)
{
  const size_t shortCapacity = 10;

  const size_t shortVectorLength = 6;
  float shortTestVector[shortVectorLength] = {1.1f,2.2f,3.3f,4.4f,5.5f,6.6f};
  float shortAnswer[shortVectorLength];

  float data;

  /* Create Instanzes */
  RingBuffer* buffer = RingBuffer_create(shortCapacity);
  Channel* channel = Channel_create(buffer);

  EXPECT_EQ(channel->poll(channel), -1);
  bool isRunning = channel->setStateRunning(channel);
  EXPECT_EQ(isRunning, false);
  bool isStopped = channel->setStateStopped(channel);
  EXPECT_EQ(isStopped, false);

  /* Configure channel */
  channel->setPollAddress(channel, &data);
  isRunning = channel->setStateRunning(channel);
  EXPECT_EQ(isRunning, true);
  data = 5.5f;
  EXPECT_EQ(channel->poll(channel), 1);
  isStopped = channel->setStateStopped(channel);
  EXPECT_EQ(isStopped, true);
  EXPECT_EQ(channel->poll(channel), -1);
}
