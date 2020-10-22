/*!*****************************************************************************
 * @file         Trigger.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 * 
 ******************************************************************************/
#include <se-lib-c/stream/IFloatStream.h>

#include "Scope/Core/Timestamper.h"
#include "Scope/Core/Trigger.h"
#include "Scope/Core/Channel.h"
#include "Scope/Core/ScopeTypes.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

struct __TriggerPrivateData;

/******************************************************************************
 Trigger definitions
******************************************************************************/
typedef enum {POLL_BUFFER, SWAP_BUFFER} BUFFER_TYPE;

typedef struct TriggerStrategyStruct{
    void (* start)(TriggerHandle self);
    bool (* stop)(TriggerHandle self);
    void (* fillUp)(TriggerHandle self);
    void (* detecting)(TriggerHandle self);

} TriggerStrategy;

typedef enum{
    TRIGGER_IDLE, TRIGGER_DETECTING, TRIGGER_FILLUP, TRIGGER_CLEANUP, TRIGGER_PAUSED
} TRIGGER_STATES;
/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __TriggerPrivateData{
    float level;
    bool edge;
    uint32_t triggerIndexes[2];
    IFloatStreamHandle stream;
    TriggerStrategy triggerStrategies[3];
    TriggerStrategy activeStrategy;
    uint32_t activeChannelId;

    ChannelHandle* channels;
    size_t amountOfChannels;
    bool isTriggereds[2];

    bool* channelIsRunning;

    TRIGGER_STATES state;
    TRIGGER_MODE mode;

    uint32_t fillUpPollCount;

    size_t channelCapacity;
    TimestamperHandle timestamper;
} TriggerPrivateData;

/* Sets a new state */
static void setState(TriggerHandle self, TRIGGER_STATES state);

/* Returns the state of the channel */
static TRIGGER_STATES getState(TriggerHandle self);

/* Checks if the trigger criteria matches the current data */
static bool checkCurrentData(TriggerHandle self, const float* triggerData);

/* Fetches the current trigger values from the stream */
static bool getTriggerData(TriggerHandle self, float* data);

/* Checks the given configuration data for possible errors and return false
 * if errors were found */
static bool configSanityCheck(TriggerHandle self, TriggerConfiguration conf);

/* Returns the right trigger strategy */
static TriggerStrategy getTriggerStrategy(TriggerHandle self, TRIGGER_MODE mode);

/* Configures the trigger */
static void writeConfig(TriggerHandle self, TriggerConfiguration conf);

static void safeChannelStates(TriggerHandle self);

static void restoreChannelStates(TriggerHandle self);

static void stopChannelsAndTimestamp(TriggerHandle self);

static void resetBuffers(TriggerHandle self);

static bool swapBuffers(TriggerHandle self);

/******************************************************************************
 Trigger strategies
******************************************************************************/
static void startNever(TriggerHandle self);

static void startWhenPaused(TriggerHandle self);

static void detectNever(TriggerHandle self);

static void detectNormal(TriggerHandle self);

static void fillUpTillChannelFull(TriggerHandle self);

static void fillUpIfTriggered(TriggerHandle self);

// static bool stopWithoutStoppingChannels(TriggerHandle self);

static bool stopIntoPause(TriggerHandle self);

static bool stopIntoIdle(TriggerHandle self);

/******************************************************************************
 Private functions
******************************************************************************/
static bool swapBuffers(TriggerHandle self){

    if(Channel_swapIsPending(self->channels[self->activeChannelId]) == false){
        return false;
    }

    if(Timestamper_swapIsPending(self->timestamper) == false){
        return false;
    }

    for(size_t i = 0; i < self->amountOfChannels; ++i){
        Channel_swapBuffers(self->channels[i]);
    }
    Timestamper_swapBuffers(self->timestamper);

    self->isTriggereds[SWAP_BUFFER] = self->isTriggereds[POLL_BUFFER];
    self->triggerIndexes[SWAP_BUFFER] = self->triggerIndexes[POLL_BUFFER];

    return true;
}

static void stopChannelsAndTimestamp(TriggerHandle self){
    for(size_t i = 0; i < self->amountOfChannels; ++i){
        Channel_setStateStopped(self->channels[i]);
    }
    Timestamper_setStateStopped(self->timestamper);
}

static void resetBuffers(TriggerHandle self){
    for(size_t i = 0; i < self->amountOfChannels; ++i){
        Channel_clear(self->channels[i]);
    }
    Timestamper_clear(self->timestamper);
}

static void safeChannelStates(TriggerHandle self){
    for(size_t i = 0; i < self->amountOfChannels; i++){
        self->channelIsRunning[i] = Channel_isRunning(self->channels[i]);
    }
    Timestamper_setStateRunning(self->timestamper);
}

static void restoreChannelStates(TriggerHandle self){
    for(size_t i = 0; i < self->amountOfChannels; i++){
        if(self->channelIsRunning[i]){
            Channel_setStateRunning(self->channels[i]);
        }
    }
    Timestamper_setStateRunning(self->timestamper);
}

static void startWhenPaused(TriggerHandle self){
    restoreChannelStates(self);
    setState(self, TRIGGER_DETECTING);
}

static void startNever(TriggerHandle self){
    (void)(self);
}

static void detectNever(TriggerHandle self){
    self->isTriggereds[POLL_BUFFER] = false;
    setState(self, TRIGGER_FILLUP);
}

static void detectNormal(TriggerHandle self){

    ChannelHandle activeChannel = self->channels[self->activeChannelId];

    /* Mache sure the channels is at least half filled. If this isn't the case, the resolution of the scope suffers */
    if(Channel_getAmountOfUsedPollData(activeChannel) < (self->channelCapacity / 2)){
        return;
    }

    float triggerData[2];
    bool streamSuccessfull = getTriggerData(self, triggerData);

    if(streamSuccessfull == false){
        return;
    }

    self->isTriggereds[POLL_BUFFER] = checkCurrentData(self, triggerData);

    if(self->isTriggereds[POLL_BUFFER] == false){
        return;
    }

    self->triggerIndexes[POLL_BUFFER] = Timestamper_getCurrentTime(self->timestamper);
    setState(self, TRIGGER_FILLUP);
}

static void fillUpTillChannelFull(TriggerHandle self){

    ChannelHandle activeChannel = self->channels[self->activeChannelId];

    if(Channel_getAmountOfUsedPollData(activeChannel) < self->channelCapacity){
        return;
    }

	safeChannelStates(self);
    setState(self, TRIGGER_CLEANUP);
}

static void fillUpIfTriggered(TriggerHandle self){

    self->fillUpPollCount++;

    if(Channel_getAmountOfUsedPollData(self->channels[self->activeChannelId]) < self->channelCapacity){
        return;
    }

    if(self->fillUpPollCount < ((self->channelCapacity / 2))){
        return;
    }
		
	safeChannelStates(self);
    setState(self, TRIGGER_CLEANUP);
}

// static bool stopWithoutStoppingChannels(TriggerHandle self){
//
//     if(swapBuffers(self) == false){
//         return false;
//     }
//
//     setState(self, TRIGGER_PAUSED);
//     self->fillUpPollCount = 0;
//
//     return true;
// }

static bool stopIntoPause(TriggerHandle self){

    stopChannelsAndTimestamp(self);

    if(swapBuffers(self) == false){
        return false;
    }

    setState(self, TRIGGER_PAUSED);
    self->fillUpPollCount = 0;

    return true;
}

static bool stopIntoIdle(TriggerHandle self){

    stopChannelsAndTimestamp(self);

    if(swapBuffers(self) == false){
        return false;
    }

    setState(self, TRIGGER_IDLE);
    self->fillUpPollCount = 0;

    return true;
}

static bool checkCurrentData(TriggerHandle self, const float* triggerData){

    const float dataCurrent = triggerData[CHANNEL_OLD_DATA];
    const float dataLast = triggerData[CHANNEL_CURRENT_DATA];
    const float triggerLevel = self->level;
    const bool edge = dataCurrent > dataLast;

    if(dataCurrent == dataLast){
        return false;
    }

    if(edge != self->edge){
        return false;
    }

    /* Sort the two values to check if the triggerLevel is between them */
    const float levelLower = dataCurrent > dataLast ? dataLast : dataCurrent;
    const float levelHigher = dataCurrent > dataLast ? dataCurrent : dataLast;
    if(triggerLevel < levelLower || triggerLevel > levelHigher){
        return false;
    }

    return true;
}

static bool getTriggerData(TriggerHandle self, float* data){

    if(self->stream->length(self->stream) != 2){
        return false;
    }

    self->stream->read(self->stream, data, 2);

    return true;
}

static bool configSanityCheck(TriggerHandle self, TriggerConfiguration conf){
    if((conf.mode != TRIGGER_NORMAL)
       && (conf.mode != TRIGGER_CONTINUOUS)
       && (conf.mode != TRIGGER_ONESHOT)){
        return false;
    }
    if(conf.channelId >= self->amountOfChannels){
        return false;
    }
    return true;
}

static TriggerStrategy getTriggerStrategy(TriggerHandle self, TRIGGER_MODE mode){
    return self->triggerStrategies[mode];
}

static void writeConfig(TriggerHandle self, TriggerConfiguration conf){
    self->level = conf.level;
    self->edge = conf.edge;
    self->mode = conf.mode;
    self->stream = Channel_getTriggerDataStream(self->channels[conf.channelId]);
    self->activeStrategy = getTriggerStrategy(self, conf.mode);
    self->activeChannelId = conf.channelId;
}

static void setState(TriggerHandle self, TRIGGER_STATES state){
    self->state = state;
}

static TRIGGER_STATES getState(TriggerHandle self){
    return self->state;
}

/******************************************************************************
 Public functions
******************************************************************************/
TriggerHandle Trigger_create(ChannelHandle* channels, size_t amountOfChannels, \
                                size_t channelCapacity, TimestamperHandle timestamper){

    TriggerHandle self = malloc(sizeof(TriggerPrivateData));
    assert(self);
    self->channels = malloc(sizeof(ChannelHandle) * amountOfChannels);
    assert(self->channels);
    self->channelIsRunning = malloc(sizeof(bool) * amountOfChannels);
    assert(self->channelIsRunning);
    self->channels = channels;
    self->amountOfChannels = amountOfChannels;
    self->channelCapacity = channelCapacity;
    self->timestamper = timestamper;
    self->isTriggereds[SWAP_BUFFER] = false;
    self->isTriggereds[POLL_BUFFER] = false;

    /* Continious trigger strategy */
    TriggerStrategy continuous;
    continuous.start = &startWhenPaused;
    continuous.detecting = &detectNever;
    continuous.fillUp = &fillUpTillChannelFull;
    continuous.stop = &stopIntoPause;

    /* Normal trigger strategy */
    TriggerStrategy normal;
    normal.start = &startWhenPaused;
    normal.detecting = &detectNormal;
    normal.fillUp = &fillUpIfTriggered;
    normal.stop = &stopIntoPause;

    /* Normal trigger strategy */
    TriggerStrategy oneshot;
    oneshot.start = &startNever;
    oneshot.detecting = &detectNormal;
    oneshot.fillUp = &fillUpIfTriggered;
    oneshot.stop = &stopIntoIdle;

    self->triggerStrategies[TRIGGER_CONTINUOUS] = continuous;
    self->triggerStrategies[TRIGGER_NORMAL] = normal;
    self->triggerStrategies[TRIGGER_ONESHOT] = oneshot;
    self->activeChannelId = 0;

    self->activeStrategy = continuous;

    Trigger_clear(self);
    Trigger_deactivate(self);
    return self;
}

void Trigger_destroy(TriggerHandle self){
    free(self->channelIsRunning);
    self->channelIsRunning = NULL;
    free(self->channels);
    self->channels = NULL;
    free(self);
    self = NULL;
}

uint32_t Trigger_getTriggerIndex(TriggerHandle self){
    return self->triggerIndexes[SWAP_BUFFER];
}

bool Trigger_configure(TriggerHandle self, TriggerConfiguration conf){

    if(!configSanityCheck(self, conf)){
        return false;
    }
    writeConfig(self, conf);
    return true;
}

bool Trigger_run(TriggerHandle self){

    if(getState(self) == TRIGGER_IDLE){
        return false;
    }
    if(getState(self) == TRIGGER_PAUSED){
        self->activeStrategy.start(self);
        Trigger_clear(self);
    } else if(getState(self) == TRIGGER_DETECTING){
        self->activeStrategy.detecting(self);
    } else if(getState(self) == TRIGGER_FILLUP){
        self->activeStrategy.fillUp(self);
    } else if(getState(self) == TRIGGER_CLEANUP){
        return self->activeStrategy.stop(self);
    }
    return false;
}

bool Trigger_isTriggered(TriggerHandle self){
    return self->isTriggereds[SWAP_BUFFER];
}

uint32_t Trigger_getChannelId(TriggerHandle self){
    return self->activeChannelId;
}

void Trigger_activate(TriggerHandle self){
    restoreChannelStates(self);
    setState(self, TRIGGER_DETECTING);
    resetBuffers(self);
    Trigger_clear(self);
}

void Trigger_deactivate(TriggerHandle self){
    safeChannelStates(self);
    stopChannelsAndTimestamp(self);
    setState(self, TRIGGER_IDLE);
    self->fillUpPollCount = 0;
}

TRIGGER_MODE Trigger_getTriggerMode(TriggerHandle self){
    return self->mode;
}

void Trigger_clear(TriggerHandle self){
    self->isTriggereds[POLL_BUFFER] = false;
    self->triggerIndexes[POLL_BUFFER] = 0;
    self->fillUpPollCount = 0;
}
