#include <gtest/gtest.h>
#include <gmock/gmock.h>

extern "C" {
    #include "channel.h"
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
