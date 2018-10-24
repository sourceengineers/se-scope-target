#include <gtest/gtest.h>
#include <gmock/gmock.h>

extern "C" {
    #include <Scope/Trigger.h>
    #include <Scope/GeneralPurpose/FloatStream.h>
}
using namespace std;

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

static void streamGetData(IFloatStreamHandle iFloatStream, float* data, const size_t length){
  globalFloatStream[CHANNEL_CURRENT_DATA] = globalTriggerDataActive;
  globalFloatStream[CHANNEL_OLD_DATA] = globalTriggerDataOld;
}

TEST(Trigger, test_normal)
{
  IFloatStream stream = *FloatStream_getFloatStream(FloatStream_create(4));
  
  TriggerHandle trigger = Trigger_create();
  TriggerConfiguration conf = {.level = 6.6f, .edge = TRIGGER_EDGE_POSITIVE, .stream = stream, .mode = TRIGGER_NORMAL, .channelId = 1};

  stream.flush(&stream);
  stream.writeData(&stream, 1.1f);
  stream.writeData(&stream, 5.5f);
  Trigger_configure(trigger, conf);
  bool isTriggered = Trigger_run(trigger, 1);

  stream.flush(&stream);
  stream.writeData(&stream, 1.1f);
  stream.writeData(&stream, 5.5f);
  conf.level = 4.4f;
  Trigger_configure(trigger, conf);
  isTriggered = Trigger_run(trigger, 1);
  ASSERT_EQ(isTriggered, true);

  stream.flush(&stream);
  stream.writeData(&stream, 5.5f);
  stream.writeData(&stream, 1.1f);
  conf.level = 3.3f;
  Trigger_configure(trigger, conf);
  isTriggered = Trigger_run(trigger, 1);  
  ASSERT_EQ(isTriggered, false);

  stream.flush(&stream);
  stream.writeData(&stream, 5.5f);
  stream.writeData(&stream, 1.1f);
  conf.level = 3.3f;
  conf.edge = TRIGGER_EDGE_NEGATIVE;
  Trigger_configure(trigger, conf);
  isTriggered = Trigger_run(trigger, 1);  
  ASSERT_EQ(isTriggered, true);    

  stream.flush(&stream);
  stream.writeData(&stream, 5.5f);
  stream.writeData(&stream, 1.1f);
  conf.level = 6.6f;
  conf.edge = TRIGGER_EDGE_NEGATIVE;
  Trigger_configure(trigger, conf);
  isTriggered = Trigger_run(trigger, 1);  
  ASSERT_EQ(isTriggered, false);  

  stream.flush(&stream);
  stream.writeData(&stream, -5.5f);
  stream.writeData(&stream, -1.1f);
  conf.level = -4.4f;
  conf.edge = TRIGGER_EDGE_NEGATIVE;
  Trigger_configure(trigger, conf);
  isTriggered = Trigger_run(trigger, 1);  
  ASSERT_EQ(isTriggered, false);  

  stream.flush(&stream);
  stream.writeData(&stream, -5.5f);
  stream.writeData(&stream, -1.1f);
  conf.level = -4.4f;
  conf.edge = TRIGGER_EDGE_POSITIVE;
  Trigger_configure(trigger, conf);
  isTriggered = Trigger_run(trigger, 1);  
  ASSERT_EQ(isTriggered, true);

  stream.flush(&stream);
  stream.writeData(&stream, -5.5f);
  stream.writeData(&stream, -1.1f);
  conf.level = -4.4f;
  conf.edge = TRIGGER_EDGE_POSITIVE;
  Trigger_configure(trigger, conf);
  isTriggered = Trigger_run(trigger, 1);  
  ASSERT_EQ(isTriggered, true);  

  stream.flush(&stream);
  stream.writeData(&stream, -5.5f);
  stream.writeData(&stream, -1.1f);
  conf.level = -6.6f;
  conf.edge = TRIGGER_EDGE_POSITIVE;
  Trigger_configure(trigger, conf);
  isTriggered = Trigger_run(trigger, 1);  
  ASSERT_EQ(isTriggered, false);  

  stream.flush(&stream);
  stream.writeData(&stream, -5.5f);
  stream.writeData(&stream, 5.5f);
  conf.level = 3.3f;
  conf.edge = TRIGGER_EDGE_POSITIVE;
  Trigger_configure(trigger, conf);
  isTriggered = Trigger_run(trigger, 1);  
  ASSERT_EQ(isTriggered, true);

  stream.flush(&stream);
  stream.writeData(&stream, -5.5f);
  stream.writeData(&stream, 5.5f);
  conf.level = -3.3f;
  conf.edge = TRIGGER_EDGE_POSITIVE;
  Trigger_configure(trigger, conf);
  isTriggered = Trigger_run(trigger, 1);  
  ASSERT_EQ(isTriggered, true);  
}

TEST(Trigger, test_continuous)
{
  IFloatStream stream;
  stream.length = &streamGetSize;
  stream.read = &streamGetData;
  
  TriggerHandle trigger = Trigger_create();
  TriggerConfiguration conf = {.stream = stream, .mode = TRIGGER_CONTINUOUS};
  
  bool isTriggered = Trigger_run(trigger, 1);

  ASSERT_EQ(isTriggered, false);  

}
