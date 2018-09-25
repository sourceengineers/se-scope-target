#include <gtest/gtest.h>
#include <gmock/gmock.h>

extern "C" {
    #include "CommandRunning.h"
    
}



TEST(CommandRunning, test_command)
{
  const size_t testElements = 3;
  float data = 5.5f;
  RingBufferHandle buffers[testElements];
  ChannelHandle channels[testElements];
  CHANNEL_STATES newStates[testElements] = {CHANNEL_RUNNING, CHANNEL_STOPPED, CHANNEL_NONE};
  
  CHANNEL_STATES changedStates[3] = {CHANNEL_NONE,CHANNEL_NONE,CHANNEL_NONE};
  
  for (size_t i = 0; i < testElements; i++) {
    buffers[i] = RingBuffer_create(10);
    channels[i] = Channel_create(buffers[i]);
    Channel_setPollAddress(channels[i], &data, FLOAT);
    Channel_setStateRunning(channels[i]);
  }

  CommandRunningHandle commandRunning = CommandRunning_create(channels, 3);
  ICommandHandle iCommandRunning = CommandRunning_getICommand(commandRunning);  
  iCommandRunning->setCommandAttribute(iCommandRunning, &newStates);
  iCommandRunning->run(iCommandRunning);

  for (size_t i = 0; i < testElements; i++) {
    changedStates[i] = Channel_getState(channels[i]);
  }
  
  ASSERT_THAT(changedStates, testing::ElementsAre(CHANNEL_RUNNING, CHANNEL_STOPPED, CHANNEL_RUNNING));
  
  
  

}
