#include <gtest/gtest.h>
#include <gmock/gmock.h>

extern "C" {
    #include <scope/trigger.h>
}

/* Mocking of the channel */
float globalTriggerDataActive;
float globalTriggerDataOld;

size_t MockGetTriggerData(Channel* self, float* triggerData){
  triggerData[0] = globalTriggerDataActive;
  triggerData[1] = globalTriggerDataOld;
  return 2;
}

TEST(Trigger, test_normal)
{
  Channel* channel;
  Trigger* trigger = Trigger_create();
  channel->getTriggerData = &MockGetTriggerData;
  TriggerConfiguration conf = {.level = 6.6f, .edge = TR_EDGE_POSITIVE, .channel = channel, .mode = TR_NORMAL};

  
  /* Data in positive range. Trigger positive edge and positive level */
  globalTriggerDataActive = 5.5f;
  globalTriggerDataOld = 1.1f;  
  trigger->configure(trigger, conf);
  bool isTriggered = trigger->trigger(trigger, 1);
  
  /* Data in positive range. Trigger positive edge and level lower */
  globalTriggerDataActive = 5.5f;
  globalTriggerDataOld = 1.1f;  
  conf.level = 4.4f;
  trigger->configure(trigger, conf);
  isTriggered = trigger->trigger(trigger, 1);  
  ASSERT_EQ(isTriggered, true);
  
  /* Data in positive range, negative edge. Trigger positive edge */
  globalTriggerDataActive = 1.1f;
  globalTriggerDataOld = 5.5f;  
  conf.level = 3.3f;
  trigger->configure(trigger, conf);
  isTriggered = trigger->trigger(trigger, 1);  
  ASSERT_EQ(isTriggered, false);
  
  globalTriggerDataActive = 1.1f;
  globalTriggerDataOld = 5.5f;  
  conf.level = 3.3f;
  conf.edge = TR_EDGE_NEGATIVE;
  trigger->configure(trigger, conf);
  isTriggered = trigger->trigger(trigger, 1);  
  ASSERT_EQ(isTriggered, true);    
  
  globalTriggerDataActive = 1.1f;
  globalTriggerDataOld = 5.5f;  
  conf.level = 6.6f;
  conf.edge = TR_EDGE_NEGATIVE;
  trigger->configure(trigger, conf);
  isTriggered = trigger->trigger(trigger, 1);  
  ASSERT_EQ(isTriggered, false);  
  
  globalTriggerDataActive = -1.1f;
  globalTriggerDataOld = -5.5f;  
  conf.level = -4.4f;
  conf.edge = TR_EDGE_NEGATIVE;
  trigger->configure(trigger, conf);
  isTriggered = trigger->trigger(trigger, 1);  
  ASSERT_EQ(isTriggered, false);  

  globalTriggerDataActive = -5.5f;
  globalTriggerDataOld = -1.1f;  
  conf.level = -4.4f;
  conf.edge = TR_EDGE_POSITIVE;
  trigger->configure(trigger, conf);
  isTriggered = trigger->trigger(trigger, 1);  
  ASSERT_EQ(isTriggered, false);  
  
  globalTriggerDataActive = -1.1f;
  globalTriggerDataOld = -5.5f;  
  conf.level = -4.4f;
  conf.edge = TR_EDGE_POSITIVE;
  trigger->configure(trigger, conf);
  isTriggered = trigger->trigger(trigger, 1);  
  ASSERT_EQ(isTriggered, true);  
  
  globalTriggerDataActive = -1.1f;
  globalTriggerDataOld = -5.5f;  
  conf.level = -6.6f;
  conf.edge = TR_EDGE_POSITIVE;
  trigger->configure(trigger, conf);
  isTriggered = trigger->trigger(trigger, 1);  
  ASSERT_EQ(isTriggered, false);  
  
  globalTriggerDataActive = 5.5f;
  globalTriggerDataOld = -5.5f;  
  conf.level = 3.3f;
  conf.edge = TR_EDGE_POSITIVE;
  trigger->configure(trigger, conf);
  isTriggered = trigger->trigger(trigger, 1);  
  ASSERT_EQ(isTriggered, true);  

  globalTriggerDataActive = 5.5f;
  globalTriggerDataOld = -5.5f;  
  conf.level = -3.3f;
  conf.edge = TR_EDGE_POSITIVE;
  trigger->configure(trigger, conf);
  isTriggered = trigger->trigger(trigger, 1);  
  ASSERT_EQ(isTriggered, true);  
}

TEST(Trigger, test_continuous)
{
  Channel* channel;
  Trigger* trigger = Trigger_create();
  channel->getTriggerData = &MockGetTriggerData;
  TriggerConfiguration conf = {.channel = channel, .mode = TR_CONTINUOUS};
  
  bool isTriggered = trigger->trigger(trigger, 1);

  ASSERT_EQ(isTriggered, false);  

}
