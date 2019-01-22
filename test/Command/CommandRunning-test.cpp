#include <gtest/gtest.h>
#include <gmock/gmock.h>

extern "C" {
    #include <Scope/Parser/Command/CommandRunning.h>
}

using namespace std;

TEST(CommandRunning, test_command)
{
  const size_t testElements = 5;
  const size_t amountChannelChanged = 3;
  float data = 5.5f;
  ChannelHandle channels[testElements];
  
  CHANNEL_STATES newStates[amountChannelChanged] = {CHANNEL_RUNNING, CHANNEL_STOPPED, CHANNEL_RUNNING};
  
  CHANNEL_STATES changedStates[testElements] = {CHANNEL_NONE,CHANNEL_NONE,CHANNEL_NONE,CHANNEL_NONE,CHANNEL_NONE};
  uint32_t changedChannels[amountChannelChanged] = {0, 2, 4};
  
  
  CommandRunningConf conf;
  conf.newStates = newStates;
  conf.numberOfChangedChannels = amountChannelChanged;
  conf.changedChannels = changedChannels;

  /* Set initial state to control */
  for (size_t i = 0; i < testElements; i++) {
    channels[i] = Channel_create(10);
    Channel_setPollAddress(channels[i], &data, FLOAT);
    Channel_setStateRunning(channels[i]);
  }
  Channel_setStateStopped(channels[0]);

  CommandRunningHandle commandRunning = CommandRunning_create(NULL);
  CommandRunning_setAttributes(commandRunning, conf);

  ICommandHandle command = CommandRunning_getICommand(commandRunning);
  command->run(command);

  for (size_t i = 0; i < testElements; i++) {
    changedStates[i] = Channel_getState(channels[i]);
  }
  
  ASSERT_THAT(changedStates, testing::ElementsAre(CHANNEL_RUNNING, CHANNEL_RUNNING, CHANNEL_STOPPED, CHANNEL_RUNNING, CHANNEL_RUNNING));
}
