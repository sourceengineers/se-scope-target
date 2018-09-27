#include <gtest/gtest.h>
#include <gmock/gmock.h>

extern "C" {
    #include "CommandTrigger.h"
}

/* Mocking of the stream */
float* globalFloatStream;

float globalTriggerDataActive;
float globalTriggerDataOld;

/* Functions for the IFloatStream interface */
static size_t streamGetSize(IFloatStreamHandle iFloatStream){
  return 2;
}

static void streamOpen(IFloatStreamHandle iFloatStream, float* floatStream){
  globalFloatStream = floatStream;
}

static void streamClose(IFloatStreamHandle iFloatStream){
  globalFloatStream = NULL;
}

static size_t streamGetData(IFloatStreamHandle iFloatStream){
  globalFloatStream[CHANNEL_CURRENT_DATA] = globalTriggerDataActive;
  globalFloatStream[CHANNEL_OLD_DATA] = globalTriggerDataOld;
  return 2;
}

TEST(CommandTrigger, test_command)
{
  IFloatStream stream;
  stream.getSize = &streamGetSize;
  stream.getStream = &streamGetData;
  stream.open = &streamOpen;
  stream.close = &streamClose;
  
  TriggerHandle trigger = Trigger_create();
  TriggerConfiguration conf = {.level = 4.4f, .edge = TRIGGER_EDGE_POSITIVE, .stream = stream, .mode = TRIGGER_NORMAL};

  globalTriggerDataActive = 5.5f;
  globalTriggerDataOld = 1.1f;  

  CommandTriggerHandle commandTrigger = CommandTrigger_create(trigger);
  ICommand* iCommand = CommandTrigger_getICommand(commandTrigger);
  
  iCommand->setCommandAttribute(iCommand, &conf);
  iCommand->run(iCommand);
  
  bool isTriggered = Trigger_run(trigger, 1); 
  
  ASSERT_EQ(isTriggered, true);

}
