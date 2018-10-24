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
  FloatRingBufferHandle buffer = FloatRingBuffer_create(shortCapacity);
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
  stream->read(stream, shortAnswer, shortVectorLength);

  ASSERT_THAT(shortAnswer, testing::ElementsAre(1.1f,2.2f,3.3f,4.4f,5.5f,6.6f));
}


TEST(Channel, test_data_types)
{
  const size_t shortCapacity = 10;

  const size_t shortVectorLength = 6;
  float shortAnswer[shortVectorLength];

  /* Create Instanzes */
  FloatRingBufferHandle buffer = FloatRingBuffer_create(shortCapacity);
  ChannelHandle channel = Channel_create(buffer);
  IFloatStreamHandle stream = Channel_getRingBufferFloatStream(channel);

  float data = 5.5f;
  /* Configure channel */
  Channel_setPollAddress(channel, &data, FLOAT);
  Channel_setStateRunning(channel);
  Channel_poll(channel);
  float answer = stream->readData(stream);
  EXPECT_EQ(answer, 5.5f);
  
 /* double doubleData = 5.5f;
  Channel_setPollAddress(channel, &doubleData, DOUBLE);
  Channel_poll(channel);
  stream->getStream(stream);
  EXPECT_EQ(shortAnswer[0], 5.5f);*/
  
  uint8_t uint8Data = 12;
  Channel_setPollAddress(channel, &uint8Data, UINT8);
  Channel_poll(channel);
  answer = stream->readData(stream);
  EXPECT_EQ((uint8_t) answer, 12);
  
  uint16_t uint16Data = 12;  
  Channel_setPollAddress(channel, &uint16Data, UINT16);
  Channel_poll(channel);
  answer = stream->readData(stream);
  EXPECT_EQ((uint16_t) answer, 12);
  
  uint32_t uint32Data = 12;  
  Channel_setPollAddress(channel, &uint32Data, UINT32);
  Channel_poll(channel);
  answer = stream->readData(stream);
  EXPECT_EQ((uint32_t) answer, 12);

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
  FloatRingBufferHandle buffer = FloatRingBuffer_create(shortCapacity);
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

  float data;

  /* Create Instanzes */
  FloatRingBufferHandle buffer = FloatRingBuffer_create(shortCapacity + 2);
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

  size_t readData;
  readData = stream->length(stream);
  ASSERT_EQ(readData, 2);

  float triggerData[2];
  stream->read(stream, triggerData, 2);

  ASSERT_THAT(triggerData, testing::ElementsAre(5.5f,6.6f));

}
