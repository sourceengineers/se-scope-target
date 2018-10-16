#include <gtest/gtest.h>
#include <gmock/gmock.h>

extern "C" {
    #include "CommandRunning.h"
}

TEST(CommandRunning, test_command)
{
  const size_t testElements = 5;
  const size_t amountChannelChanged = 3;
  float data = 5.5f;
  FloatRingBufferHandle buffers[testElements];
  ChannelHandle channels[testElements];
  
  CHANNEL_STATES newStates[amountChannelChanged] = {CHANNEL_RUNNING, CHANNEL_STOPPED, CHANNEL_RUNNING};
  
  CHANNEL_STATES changedStates[testElements] = {CHANNEL_NONE,CHANNEL_NONE,CHANNEL_NONE,CHANNEL_NONE,CHANNEL_NONE};
  int changedChannels[amountChannelChanged] = {0, 2, 4};
  
  
  CommandRunningConf conf = {.newStates = newStates, .changedChannels = changedChannels, .numberOfChangedChannels = amountChannelChanged};
  
  /* Set initial state to control */
  for (size_t i = 0; i < testElements; i++) {
    buffers[i] = FloatRingBuffer_create(10);
    channels[i] = Channel_create(buffers[i]);
    Channel_setPollAddress(channels[i], &data, FLOAT);
    Channel_setStateRunning(channels[i]);
  }
  Channel_setStateStopped(channels[0]);

  CommandRunningHandle commandRunning = CommandRunning_create(channels, testElements);
  ICommandHandle iCommandRunning = CommandRunning_getICommand(commandRunning);  
  iCommandRunning->setCommandAttribute(iCommandRunning, &conf);
  iCommandRunning->run(iCommandRunning);

  for (size_t i = 0; i < testElements; i++) {
    changedStates[i] = Channel_getState(channels[i]);
  }
  
  ASSERT_THAT(changedStates, testing::ElementsAre(CHANNEL_RUNNING, CHANNEL_RUNNING, CHANNEL_STOPPED, CHANNEL_RUNNING, CHANNEL_RUNNING));
}
