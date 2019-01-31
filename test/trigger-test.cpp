#include <gtest/gtest.h>
#include <gmock/gmock.h>

extern "C" {
#include <Scope/Core/Trigger.h>
#include <Scope/GeneralPurpose/BufferedFloatStream.h>
}
using namespace std;


bool testCondition(float* var, float var1, float var2, TriggerConfiguration conf, ChannelHandle channel, TriggerHandle trigger){

    Channel_clear(channel);
    Trigger_clear(trigger);

    Trigger_configure(trigger, conf);
    Trigger_activate(trigger);

    *var = 0;
    for(int i = 0; i < (Channel_getCapacity(channel) / 2)-2 ; ++i){
        Channel_poll(channel);
        Trigger_run(trigger);
    }

    *var = var1;
    Channel_poll(channel);
    Trigger_run(trigger);
    *var = var2;
    Channel_poll(channel);
    Trigger_run(trigger);

    bool isTriggered = Trigger_isTriggered(trigger);
    Trigger_deactivate(trigger);
    return isTriggered;

}

TEST(Trigger, test_normal){
    ChannelHandle channel = Channel_create(200);
    float var = 0;
    Channel_setPollAddress(channel, &var, FLOAT);
    Channel_setStateRunning(channel);
    uint32_t timestamp = 0;
    TimestamperHandle timestamper = Timestamper_create(200, &timestamp);

    TriggerHandle trigger = Trigger_create(&channel, 1, 200, timestamper);
    TriggerConfiguration conf = {.level = 6.6f, .edge = TRIGGER_EDGE_POSITIVE, .mode = TRIGGER_ONESHOT, .channelId = 0};
    Trigger_activate(trigger);

    bool isTriggered = testCondition(&var, 1.1f, 5.5f, conf, channel, trigger);
    EXPECT_EQ(isTriggered, false);

    conf.level = 4.4f;
    isTriggered = testCondition(&var, 1.1f, 5.5f, conf, channel, trigger);
    EXPECT_EQ(isTriggered, true);

    conf.level = 3.3f;
    isTriggered = testCondition(&var, 5.5f, 1.1f, conf, channel, trigger);
    EXPECT_EQ(isTriggered, false);

    conf.level = 3.3f;
    conf.edge = TRIGGER_EDGE_NEGATIVE;
    isTriggered = testCondition(&var, 5.5f, 1.1f, conf, channel, trigger);
    EXPECT_EQ(isTriggered, true);

    conf.level = 6.6f;
    conf.edge = TRIGGER_EDGE_NEGATIVE;
    isTriggered = testCondition(&var, 5.5f, 1.1f, conf, channel, trigger);
    EXPECT_EQ(isTriggered, false);

    conf.level = -4.4f;
    conf.edge = TRIGGER_EDGE_NEGATIVE;
    isTriggered = testCondition(&var, -5.5f, -1.1f, conf, channel, trigger);
    EXPECT_EQ(isTriggered, false);

    conf.level = -4.4f;
    conf.edge = TRIGGER_EDGE_POSITIVE;
    isTriggered = testCondition(&var, -5.5f, -1.1f, conf, channel, trigger);
    EXPECT_EQ(isTriggered, true);

    conf.level = -6.6f;
    conf.edge = TRIGGER_EDGE_POSITIVE;
    isTriggered = testCondition(&var, -5.5f, -1.1f, conf, channel, trigger);
    EXPECT_EQ(isTriggered, false);

    conf.level = 3.3f;
    conf.edge = TRIGGER_EDGE_POSITIVE;
    isTriggered = testCondition(&var, -5.5f, 5.5f, conf, channel, trigger);
    EXPECT_EQ(isTriggered, true);

    conf.level = -3.3f;
    conf.edge = TRIGGER_EDGE_POSITIVE;
    isTriggered = testCondition(&var, -5.5f, 5.5f, conf, channel, trigger);
    EXPECT_EQ(isTriggered, true);
}

TEST(Trigger, test_continuous){
    ChannelHandle channel = Channel_create(200);
    float var = 0;
    Channel_setPollAddress(channel, &var, FLOAT);
    Channel_setStateRunning(channel);
    uint32_t timestamp = 0;
    TimestamperHandle timestamper = Timestamper_create(200, &timestamp);
    TriggerHandle trigger = Trigger_create(&channel, 1, 200, timestamper);
    TriggerConfiguration conf = {.level = 6.6f, .edge = TRIGGER_EDGE_POSITIVE, .mode = TRIGGER_CONTINUOUS, .channelId = 1};

    bool isTriggered = testCondition(&var, 1.1f, 5.5f, conf, channel, trigger);
    EXPECT_EQ(isTriggered, false);
}
