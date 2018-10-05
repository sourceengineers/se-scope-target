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
  Channel_setPollAddress(channel, &data, FLOAT);
  Channel_setStateRunning(channel);
  
  /* Simulate polling */
  for(size_t i = 0; i < shortVectorLength; i++){
    data = shortTestVector[i];
    Channel_poll(channel);
  }
  
  /* Get stream interface to test the procedure */
  IFloatStreamHandle stream = Channel_getRingBufferFloatStream(channel);
  stream->open(stream, shortAnswer);
  stream->getStream(stream);
  stream->close(stream);
  ASSERT_THAT(shortAnswer, testing::ElementsAre(1.1f,2.2f,3.3f,4.4f,5.5f,6.6f));
}


TEST(Channel, test_data_types)
{
  const size_t shortCapacity = 10;

  const size_t shortVectorLength = 6;
  float shortAnswer[shortVectorLength];

  /* Create Instanzes */
  RingBufferHandle buffer = RingBuffer_create(shortCapacity);
  ChannelHandle channel = Channel_create(buffer);
  IFloatStreamHandle stream = Channel_getRingBufferFloatStream(channel);
  stream->open(stream, shortAnswer);

  float data = 5.5f;
  /* Configure channel */
  Channel_setPollAddress(channel, &data, FLOAT);
  Channel_setStateRunning(channel);
  Channel_poll(channel);
  stream->getStream(stream);
  EXPECT_EQ(shortAnswer[0], 5.5f);
  
 /* double doubleData = 5.5f;
  Channel_setPollAddress(channel, &doubleData, DOUBLE);
  Channel_poll(channel);
  stream->getStream(stream);
  EXPECT_EQ(shortAnswer[0], 5.5f);*/
  
  uint8_t uint8Data = 12;
  Channel_setPollAddress(channel, &uint8Data, UINT8);
  Channel_poll(channel);
  stream->getStream(stream);
  EXPECT_EQ(shortAnswer[0], 12);
  
  uint16_t uint16Data = 12;  
  Channel_setPollAddress(channel, &uint16Data, UINT16);
  Channel_poll(channel);
  stream->getStream(stream);
  EXPECT_EQ(shortAnswer[0], 12);
  
  uint32_t uint32Data = 12;  
  Channel_setPollAddress(channel, &uint32Data, UINT32);
  Channel_poll(channel);
  stream->getStream(stream);
  EXPECT_EQ(shortAnswer[0], 12);

/*  uint64_t uint64Data = 12;
  Channel_setPollAddress(channel, &uint64Data, UINT64);
  Channel_poll(channel);
  stream->getStream(stream);
  EXPECT_EQ(shortAnswer[0], 12);*/
  
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
  Channel_setPollAddress(channel, &data, FLOAT);
  isRunning = Channel_setStateRunning(channel);
  EXPECT_EQ(isRunning, true);
  data = 5.5f;
  EXPECT_EQ(Channel_poll(channel), 1);
  isStopped = Channel_setStateStopped(channel);
  EXPECT_EQ(isStopped, true);
  EXPECT_EQ(Channel_poll(channel), -1);
}

TEST(Channel, test_trigger_stream)
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
  Channel_setPollAddress(channel, &data, FLOAT);
  Channel_setStateRunning(channel);
  
  /* Simulate polling */
  for(size_t i = 0; i < shortVectorLength; i++){
    data = shortTestVector[i];
    Channel_poll(channel);
  }
  
  /* Get stream interface to test the procedure */
  IFloatStreamHandle stream = Channel_getTriggerDataStream(channel);

  float triggerData[2];
  size_t readData;
  stream->open(stream, triggerData);
  readData = stream->getSize(stream);
  ASSERT_EQ(readData, 2);
  readData = stream->getStream(stream);
  ASSERT_EQ(readData, 2);
  stream->close(stream);
  
  ASSERT_THAT(triggerData, testing::ElementsAre(6.6f,5.5f));

}
