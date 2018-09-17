#include <gtest/gtest.h>
#include <gmock/gmock.h>

extern "C" {
    #include <scope/trigger.h>
}

size_t MockGetTriggerData(Channel* self, float* triggerData){
  triggerData[0] = 5.5f;
  triggerData[1] = 1.1f;
  return 2;
}

TEST(Trigger, test_normal)
{
  Channel* channel;
  channel->getTriggerData = &MockGetTriggerData;
  TriggerConfiguration normalPosEdgePosLevel = {.level = 4.4, .edge = TR_EDGE_POSITIVE, .channel = channel, .mode = TR_NORMAL};
    
  Trigger* trigger = Trigger_create();
  trigger->configure(trigger, normalPosEdgePosLevel);
  
  bool isTriggered = trigger->trigger(trigger);
  ASSERT_EQ(isTriggered, true);
}
