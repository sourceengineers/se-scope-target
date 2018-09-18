#include <gtest/gtest.h>
#include <gmock/gmock.h>

extern "C" {
    #include "Channel.h"
}

TEST(Channel, test_polling)
{
  const size_t shortCapacity = 10;

  const size_t shortVectorLength = 6;
  float shortTestVector[shortVectorLength] = {1.1f,2.2f,3.3f,4.4f,5.5f,6.6f};
  float shortAnswer[shortVectorLength];

  float data;

  /* Create Instanzes */
  RingBufferHandle buffer = RingBuffer_create(shortCapacity);
  ChannelHandle channel = Channel_create(buffer);
  
  /* Configure channel */
  Channel_setPollAddress(channel, &data);
  Channel_setStateRunning(channel);
  
  /* Simulate polling */
  for(size_t i = 0; i < shortVectorLength; i++){
    data = shortTestVector[i];
    Channel_poll(channel);
  }
  
  /* Get stream interface to test the procedure */
  IFloatStreamHandle stream = Channel_getFloatStream(channel);
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
  RingBufferHandle buffer = RingBuffer_create(shortCapacity);
  ChannelHandle channel = Channel_create(buffer);

  EXPECT_EQ(Channel_poll(channel), -1);
  bool isRunning = Channel_setStateRunning(channel);
  EXPECT_EQ(isRunning, false);
  bool isStopped = Channel_setStateStopped(channel);
  EXPECT_EQ(isStopped, false);

  /* Configure channel */
  Channel_setPollAddress(channel, &data);
  isRunning = Channel_setStateRunning(channel);
  EXPECT_EQ(isRunning, true);
  data = 5.5f;
  EXPECT_EQ(Channel_poll(channel), 1);
  isStopped = Channel_setStateStopped(channel);
  EXPECT_EQ(isStopped, true);
  EXPECT_EQ(Channel_poll(channel), -1);
}
