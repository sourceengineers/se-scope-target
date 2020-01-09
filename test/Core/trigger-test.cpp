#include <gtest/gtest.h>
#include <gmock/gmock.h>

extern "C" {
#include <Scope/Core/Trigger.h>
#include <se-lib-c/stream/BufferedFloatStream.h>
}
using namespace std;


bool testCondition(float var1, float var2, TriggerConfiguration conf){

    ChannelHandle channel = Channel_create(200);
    float var = 0;
    Channel_setPollAddress(channel, &var, SE_FLOAT);
    Channel_setStateRunning(channel);
    uint32_t timestamp = 0;
    TimestamperHandle timestamper = Timestamper_create(200, &timestamp);

    TriggerHandle trigger = Trigger_create(&channel, 1, 200, timestamper);
    Trigger_activate(trigger);
    Channel_clear(channel);
    Trigger_clear(trigger);
    Trigger_configure(trigger, conf);
    Trigger_activate(trigger);

    /* Fill up the channel until at least half full */
    var = 0;
    for(int i = 0; i < (Channel_getCapacity(channel) / 2) - 2; ++i){
        Channel_poll(channel);
        Timestamper_stamp(timestamper);
        Trigger_run(trigger);
    }

    /* Poll the two values that are to be tested */
    var = var1;
    Channel_poll(channel);
    Timestamper_stamp(timestamper);
    Trigger_run(trigger);
    var = var2;
    Channel_poll(channel);
    Timestamper_stamp(timestamper);
    Trigger_run(trigger);

    /* Fill up channel */
    var = 0;
    for(int i = 0; i < (Channel_getCapacity(channel) / 2 + 2); ++i){
        Channel_poll(channel);
        Timestamper_stamp(timestamper);
        Trigger_run(trigger);
    }

    return Trigger_isTriggered(trigger);
}

TEST(Trigger, test_normal){

    TriggerConfiguration conf = {.level = 6.6f, .edge = TRIGGER_EDGE_POSITIVE, .mode = TRIGGER_NORMAL, .channelId = 0};

    bool isTriggered = testCondition(1.1f, 5.5f, conf);
    EXPECT_EQ(isTriggered, false);

    conf.level = 4.4f;
    isTriggered = testCondition(1.1f, 5.5f, conf);
    EXPECT_EQ(isTriggered, true);

    conf.level = 3.3f;
    isTriggered = testCondition(5.5f, 1.1f, conf);
    EXPECT_EQ(isTriggered, false);

    conf.level = 3.3f;
    conf.edge = TRIGGER_EDGE_NEGATIVE;
    isTriggered = testCondition(5.5f, 1.1f, conf);
    EXPECT_EQ(isTriggered, true);

    conf.level = 6.6f;
    conf.edge = TRIGGER_EDGE_NEGATIVE;
    isTriggered = testCondition(5.5f, 1.1f, conf);
    EXPECT_EQ(isTriggered, false);

    conf.level = -4.4f;
    conf.edge = TRIGGER_EDGE_NEGATIVE;
    isTriggered = testCondition(-5.5f, -1.1f, conf);
    EXPECT_EQ(isTriggered, false);

    conf.level = -4.4f;
    conf.edge = TRIGGER_EDGE_POSITIVE;
    isTriggered = testCondition(-5.5f, -1.1f, conf);
    EXPECT_EQ(isTriggered, true);

    conf.level = -6.6f;
    conf.edge = TRIGGER_EDGE_POSITIVE;
    isTriggered = testCondition(-5.5f, -1.1f, conf);
    EXPECT_EQ(isTriggered, false);

    conf.level = 3.3f;
    conf.edge = TRIGGER_EDGE_POSITIVE;
    isTriggered = testCondition(-5.5f, 5.5f, conf);
    EXPECT_EQ(isTriggered, true);

    conf.level = -3.3f;
    conf.edge = TRIGGER_EDGE_POSITIVE;
    isTriggered = testCondition(-5.5f, 5.5f, conf);
    EXPECT_EQ(isTriggered, true);
}