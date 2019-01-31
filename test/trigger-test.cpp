#include <gtest/gtest.h>
#include <gmock/gmock.h>

extern "C" {
#include <Scope/Core/Trigger.h>
#include <Scope/GeneralPurpose/BufferedFloatStream.h>
}
using namespace std;

TEST(Trigger, test_normal){
    ChannelHandle channel = Channel_create(200);
    float var = 0;
    Channel_setPollAddress(channel, &var, FLOAT);
    Channel_setStateRunning(channel);

    TriggerHandle trigger = Trigger_create(&channel, 1);
    TriggerConfiguration conf = {.level = 6.6f, .edge = TRIGGER_EDGE_POSITIVE, .mode = TRIGGER_NORMAL, .channelId = 0};

    var = 1.1f;
    Channel_poll(channel);
    var = 5.5f;
    Channel_poll(channel);

    Trigger_configure(trigger, conf);
    bool isTriggered = Trigger_run(trigger, 1);
    EXPECT_EQ(isTriggered, false);

    var = 1.1f;
    Channel_poll(channel);
    var = 5.5f;
    Channel_poll(channel);
    conf.level = 4.4f;
    Trigger_configure(trigger, conf);
    isTriggered = Trigger_run(trigger, 1);
    EXPECT_EQ(isTriggered, true);

    var = 5.5f;
    Channel_poll(channel);
    var = 1.1f;
    Channel_poll(channel);
    conf.level = 3.3f;
    Trigger_configure(trigger, conf);
    isTriggered = Trigger_run(trigger, 1);
    EXPECT_EQ(isTriggered, false);

    var = 5.5f;
    Channel_poll(channel);
    var = 1.1f;
    Channel_poll(channel);
    conf.level = 3.3f;
    conf.edge = TRIGGER_EDGE_NEGATIVE;
    Trigger_configure(trigger, conf);
    isTriggered = Trigger_run(trigger, 1);
    EXPECT_EQ(isTriggered, true);

    var = 5.5f;
    Channel_poll(channel);
    var = 1.1f;
    Channel_poll(channel);
    conf.level = 6.6f;
    conf.edge = TRIGGER_EDGE_NEGATIVE;
    Trigger_configure(trigger, conf);
    isTriggered = Trigger_run(trigger, 1);
    EXPECT_EQ(isTriggered, false);

    var = -5.5f;
    Channel_poll(channel);
    var = -1.1f;
    Channel_poll(channel);
    conf.level = -4.4f;
    conf.edge = TRIGGER_EDGE_NEGATIVE;
    Trigger_configure(trigger, conf);
    isTriggered = Trigger_run(trigger, 1);
    EXPECT_EQ(isTriggered, false);

    var = -5.5f;
    Channel_poll(channel);
    var = -1.1f;
    Channel_poll(channel);
    conf.level = -4.4f;
    conf.edge = TRIGGER_EDGE_POSITIVE;
    Trigger_configure(trigger, conf);
    isTriggered = Trigger_run(trigger, 1);
    EXPECT_EQ(isTriggered, true);

    var = -5.5f;
    Channel_poll(channel);
    var = -1.1f;
    Channel_poll(channel);
    conf.edge = TRIGGER_EDGE_POSITIVE;
    Trigger_configure(trigger, conf);
    isTriggered = Trigger_run(trigger, 1);
    EXPECT_EQ(isTriggered, true);

    var = -5.5f;
    Channel_poll(channel);
    var = -1.1f;
    Channel_poll(channel);
    conf.level = -6.6f;
    conf.edge = TRIGGER_EDGE_POSITIVE;
    Trigger_configure(trigger, conf);
    isTriggered = Trigger_run(trigger, 1);
    EXPECT_EQ(isTriggered, false);

    var = -5.5f;
    Channel_poll(channel);
    var = 5.5f;
    Channel_poll(channel);
    conf.level = 3.3f;
    conf.edge = TRIGGER_EDGE_POSITIVE;
    Trigger_configure(trigger, conf);
    isTriggered = Trigger_run(trigger, 1);
    EXPECT_EQ(isTriggered, true);

    var = -5.5f;
    Channel_poll(channel);
    var = 5.5f;
    Channel_poll(channel);
    conf.level = -3.3f;
    conf.edge = TRIGGER_EDGE_POSITIVE;
    Trigger_configure(trigger, conf);
    isTriggered = Trigger_run(trigger, 1);
    EXPECT_EQ(isTriggered, true);
}

TEST(Trigger, test_continuous){

    TriggerHandle trigger = Trigger_create(NULL, 0);
    TriggerConfiguration conf = {.level = 6.6f, .edge = TRIGGER_EDGE_POSITIVE, .mode = TRIGGER_CONTINUOUS, .channelId = 1};

    bool isTriggered = Trigger_run(trigger, 1);

    EXPECT_EQ(isTriggered, false);

}
