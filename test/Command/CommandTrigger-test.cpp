#include <gtest/gtest.h>
#include <gmock/gmock.h>

extern "C" {
    #include <Scope/Command/CommandTrigger.h>
    #include <Scope/GeneralPurpose/BufferedFloatStream.h>
}

using namespace std;

TEST(CommandTrigger, test_command)
{
  IFloatStream stream = *BufferedFloatStream_getIFloatStream(BufferedFloatStream_create(4));

  TriggerHandle trigger = Trigger_create();
  TriggerConfiguration conf = {.level = 4.4f, .edge = TRIGGER_EDGE_POSITIVE, .mode = TRIGGER_NORMAL, .stream = stream, .channelId = 0};

  stream.writeData(&stream, 1.1f);
  stream.writeData(&stream, 5.5f);

  CommandTriggerHandle commandTrigger = CommandTrigger_create(trigger);
  ICommand* command = CommandTrigger_getICommand(commandTrigger);

  command->setCommandAttribute(command, &conf);
  command->run(command);

  bool isTriggered = Trigger_run(trigger, 1);

  ASSERT_EQ(isTriggered, true);

}
