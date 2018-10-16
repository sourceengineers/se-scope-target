#include <gtest/gtest.h>
#include <gmock/gmock.h>

extern "C" {
    #include "CommandTrigger.h"
    #include "FloatStream.h"
}

TEST(CommandTrigger, test_command)
{
  IFloatStream stream = *FloatStream_getFloatStream(FloatStream_create(4));

  TriggerHandle trigger = Trigger_create();
  TriggerConfiguration conf = {.level = 4.4f, .edge = TRIGGER_EDGE_POSITIVE, .stream = stream, .mode = TRIGGER_NORMAL};

  stream.writeData(&stream, 5.5f);
  stream.writeData(&stream, 1.1f);

  CommandTriggerHandle commandTrigger = CommandTrigger_create(trigger);
  ICommand* iCommand = CommandTrigger_getICommand(commandTrigger);
  
  iCommand->setCommandAttribute(iCommand, &conf);
  iCommand->run(iCommand);
  
  bool isTriggered = Trigger_run(trigger, 1); 
  
  ASSERT_EQ(isTriggered, true);

}
