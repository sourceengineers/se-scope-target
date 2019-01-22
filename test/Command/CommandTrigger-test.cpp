#include <gtest/gtest.h>
#include <gmock/gmock.h>

extern "C" {
    #include <Scope/Parser/Command/CommandTrigger.h>
    #include <Scope/GeneralPurpose/BufferedFloatStream.h>
    #include <Scope/Core/Trigger.h>
}

using namespace std;

IScopeHandle scope;
TriggerHandle trigger;

static void configureTrigger(IScopeHandle scope, TriggerConfiguration conf){
  Trigger_configure(trigger, conf);
}

TEST(CommandTrigger, test_command)
{
  IFloatStream stream = *BufferedFloatStream_getIFloatStream(BufferedFloatStream_create(4));

  scope->configureTrigger = &configureTrigger;

  trigger = Trigger_create(NULL, 0);
  TriggerConfiguration conf = {.level = 4.4f, .edge = TRIGGER_EDGE_POSITIVE, .mode = TRIGGER_NORMAL, .channelId = 0};

  stream.writeData(&stream, 1.1f);
  stream.writeData(&stream, 5.5f);

  CommandTriggerHandle commandTrigger = CommandTrigger_create(scope);
  CommandTrigger_setAttributes(commandTrigger, conf);
  ICommand* command = CommandTrigger_getICommand(commandTrigger);
  command->run(command);

  bool isTriggered = Trigger_run(trigger, 1);

  ASSERT_EQ(isTriggered, true);

}
