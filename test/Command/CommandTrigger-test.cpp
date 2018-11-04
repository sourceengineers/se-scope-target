#include <gtest/gtest.h>
#include <gmock/gmock.h>

extern "C" {
    #include <Scope/Command/CommandTrigger.h>
    #include <Scope/GeneralPurpose/FloatStream.h>
}

using namespace std;

TEST(CommandTrigger, test_command)
{
  IFloatStream stream = *FloatStream_getIFloatStream(FloatStream_create(4));

  TriggerHandle trigger = Trigger_create();
  TriggerConfiguration conf = {.level = 4.4f, .edge = TRIGGER_EDGE_POSITIVE, .mode = TRIGGER_NORMAL, .stream = stream, .channelId = 0};

  stream.writeData(&stream, 1.1f);
  stream.writeData(&stream, 5.5f);

  CommandTriggerHandle commandTrigger = CommandTrigger_create(trigger);
  ICommand* iCommand = CommandTrigger_getICommand(commandTrigger);

  iCommand->setCommandAttribute(iCommand, &conf);
  iCommand->run(iCommand);

  bool isTriggered = Trigger_run(trigger, 1);

  ASSERT_EQ(isTriggered, true);

}
