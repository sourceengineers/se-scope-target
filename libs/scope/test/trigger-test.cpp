#include <gtest/gtest.h>
#include <gmock/gmock.h>

extern "C" {
    #include <scope/Trigger.h>
}

/* Mocking of the channel */
float globalTriggerDataActive;
float globalTriggerDataOld;

size_t MockGetTriggerData(ChannelHandle self, float* triggerData){
  triggerData[0] = globalTriggerDataActive;
  triggerData[1] = globalTriggerDataOld;
  return 2;
}

TEST(Trigger, test_normal)
{
}
/*  ChannelHandle channel;
  TriggerHandle trigger = Trigger_create();
  channel->getTriggerData = &MockGetTriggerData;
  TriggerConfiguration conf = {.level = 6.6f, .edge = TRIGGER_EDGE_POSITIVE, .channel = channel, .mode = TRIGGER_NORMAL};

  globalTriggerDataActive = 5.5f;
  globalTriggerDataOld = 1.1f;  
  trigger->configure(trigger, conf);
  bool isTriggered = trigger->trigger(trigger, 1);
  
  globalTriggerDataActive = 5.5f;
  globalTriggerDataOld = 1.1f;  
  conf.level = 4.4f;
  trigger->configure(trigger, conf);
  isTriggered = trigger->trigger(trigger, 1);  
  ASSERT_EQ(isTriggered, true);
  
  globalTriggerDataActive = 1.1f;
  globalTriggerDataOld = 5.5f;  
  conf.level = 3.3f;
  trigger->configure(trigger, conf);
  isTriggered = trigger->trigger(trigger, 1);  
  ASSERT_EQ(isTriggered, false);
  
  globalTriggerDataActive = 1.1f;
  globalTriggerDataOld = 5.5f;  
  conf.level = 3.3f;
  conf.edge = TRIGGER_EDGE_NEGATIVE;
  trigger->configure(trigger, conf);
  isTriggered = trigger->trigger(trigger, 1);  
  ASSERT_EQ(isTriggered, true);    
  
  globalTriggerDataActive = 1.1f;
  globalTriggerDataOld = 5.5f;  
  conf.level = 6.6f;
  conf.edge = TRIGGER_EDGE_NEGATIVE;
  trigger->configure(trigger, conf);
  isTriggered = trigger->trigger(trigger, 1);  
  ASSERT_EQ(isTriggered, false);  
  
  globalTriggerDataActive = -1.1f;
  globalTriggerDataOld = -5.5f;  
  conf.level = -4.4f;
  conf.edge = TRIGGER_EDGE_NEGATIVE;
  trigger->configure(trigger, conf);
  isTriggered = trigger->trigger(trigger, 1);  
  ASSERT_EQ(isTriggered, false);  

  globalTriggerDataActive = -5.5f;
  globalTriggerDataOld = -1.1f;  
  conf.level = -4.4f;
  conf.edge = TRIGGER_EDGE_POSITIVE;
  trigger->configure(trigger, conf);
  isTriggered = trigger->trigger(trigger, 1);  
  ASSERT_EQ(isTriggered, false);  
  
  globalTriggerDataActive = -1.1f;
  globalTriggerDataOld = -5.5f;  
  conf.level = -4.4f;
  conf.edge = TRIGGER_EDGE_POSITIVE;
  trigger->configure(trigger, conf);
  isTriggered = trigger->trigger(trigger, 1);  
  ASSERT_EQ(isTriggered, true);  
  
  globalTriggerDataActive = -1.1f;
  globalTriggerDataOld = -5.5f;  
  conf.level = -6.6f;
  conf.edge = TRIGGER_EDGE_POSITIVE;
  trigger->configure(trigger, conf);
  isTriggered = trigger->trigger(trigger, 1);  
  ASSERT_EQ(isTriggered, false);  
  
  globalTriggerDataActive = 5.5f;
  globalTriggerDataOld = -5.5f;  
  conf.level = 3.3f;
  conf.edge = TRIGGER_EDGE_POSITIVE;
  trigger->configure(trigger, conf);
  isTriggered = trigger->trigger(trigger, 1);  
  ASSERT_EQ(isTriggered, true);  

  globalTriggerDataActive = 5.5f;
  globalTriggerDataOld = -5.5f;  
  conf.level = -3.3f;
  conf.edge = TRIGGER_EDGE_POSITIVE;
  trigger->configure(trigger, conf);
  isTriggered = trigger->trigger(trigger, 1);  
  ASSERT_EQ(isTriggered, true);  
}

TEST(Trigger, test_continuous)
{
  ChannelHandle channel;
  TriggerHandle trigger = Trigger_create();
  channel->getTriggerData = &MockGetTriggerData;
  TriggerConfiguration conf = {.channel = channel, .mode = TRIGGER_CONTINUOUS};
  
  bool isTriggered = trigger->trigger(trigger, 1);

  ASSERT_EQ(isTriggered, false);  

}
*/
