#include <gtest/gtest.h>
#include <gmock/gmock.h>

extern "C" {
    #include "CommandAddr.h"
}

TEST(CommandAddr, test_command)
{
  const size_t testElements = 5;
  const size_t amountChannelChanged = 3;
  float dataPointOne = 5.5f;
  
  FloatRingBufferHandle buffers[testElements];
  ChannelHandle channels[testElements];
  
  int firstChangedElements[testElements] = {0, 1, 2, 3, 4};
  void* firstAddrPointer[testElements] = {&dataPointOne, &dataPointOne, &dataPointOne, &dataPointOne, &dataPointOne};
  DATA_TYPES firstTypes[testElements] = {FLOAT, FLOAT, FLOAT, FLOAT, FLOAT};
  
  CommandAddrConf firstConfig = {.newAddresses = firstAddrPointer, .changedChannels = firstChangedElements, .numberOfChangedChannels = testElements, .types = firstTypes};
    
  /* Set initial state to control */
  for (size_t i = 0; i < testElements; i++) {
    buffers[i] = FloatRingBuffer_create(10);
    channels[i] = Channel_create(buffers[i]);
  }
  
  /* Configure all channels and check if it worked */
  CommandAddrHandle commandAddr =  CommandAddr_create(channels, testElements);
  ICommand* iCommand = CommandAddr_getICommand(commandAddr);
  iCommand->setCommandAttribute(iCommand, &firstConfig);
  iCommand->run(iCommand);
  
  /* Set initial state to control */
  for (size_t i = 0; i < testElements; i++) {
    void* currentAddrPointer = Channel_getPollAddress(channels[i]);
    ASSERT_EQ(currentAddrPointer, &dataPointOne);
  }
  
  /* Reconfigure some channels to check if only the specified channels get changed */
  float dataPointTwo = 6.6f;
  int secondChangedElements[testElements] = {2,3,4};
  void* secondAddrPointer[testElements] = {&dataPointTwo, &dataPointTwo, &dataPointTwo};
  DATA_TYPES secondTypes[testElements] = {FLOAT, FLOAT, FLOAT};
  
  CommandAddrConf secondConfig = {.newAddresses = secondAddrPointer, .changedChannels = secondChangedElements, .numberOfChangedChannels = 3, .types = secondTypes};
  iCommand->setCommandAttribute(iCommand, &secondConfig);
  iCommand->run(iCommand);

  /* Test if the first 2 channels kept their configuration */
  for (size_t i = 0; i < 2; i++) {
    void* currentAddrPointer = Channel_getPollAddress(channels[i]);
    ASSERT_EQ(currentAddrPointer, &dataPointOne);
  }
  
  /* Test if the last 3 channels changed their configuration */
  for (size_t i = 2; i < 5; i++) {
    void* currentAddrPointer = Channel_getPollAddress(channels[i]);
    ASSERT_EQ(currentAddrPointer, &dataPointTwo);
  }
}
