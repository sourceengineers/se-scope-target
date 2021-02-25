/*!*****************************************************************************
 * @file         ScopeMock.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
*******************************************************************************/

#include "Scope/GeneralPurpose/DataTypes.h"

#include "Scope/Core/IScope.h"
#include "Scope/Core/ScopeTypes.h"
#include "Scope/Core/Trigger.h"

#include <se-lib-c/container/FloatRingBuffer.h>
#include "../../ext/se-lib-c/public/se-lib-c/stream/IIntStream.h"
#include <se-lib-c/util/observer/IObserver.h>
#include "ScopeMock.h"

#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */

static bool isReadyToRun(IScopeHandle scope);

static bool isRunning(IScopeHandle scope);

static void scopePoll(IScopeHandle scope);

static void scopeClear(IScopeHandle scope);

static void scopeSetTimeIncrement(IScopeHandle scope, uint32_t timeIncrement);

static uint32_t getTimeIncrement(IScopeHandle scope);

static size_t getAmountOfChannels(IScopeHandle scope);

static IIntStreamHandle getTimestamp(IScopeHandle scope);

static void configureTrigger(IScopeHandle scope, TriggerConfiguration conf);

static void setChannelRunning(IScopeHandle scope, uint32_t idOfChangedChannel);

static void setChannelStopped(IScopeHandle scope, uint32_t idOfChangedChannel);

static void configureChannelAddress(IScopeHandle scope, void* address,

                                    uint32_t idOfChangedChannel, DATA_TYPES typeOfAddress);

static void transmit(IScopeHandle scope);

static bool run(IRunnableHandle runnable);

static TriggeredValues getTriggerData(IScopeHandle scope);

static bool channelHasToBePacked(IScopeHandle scope, uint32_t channelId);

static FloatRingBufferHandle getChannelBuffer(IScopeHandle scope, uint32_t channelId);

/******************************************************************************
 Private functions
******************************************************************************/
static bool isReadyToRun(IScopeHandle scope){
    ScopeMockHandle self = (ScopeMockHandle) scope->handle;

    self->isReadyToRunHasBeenCalled = true;
}

static bool isRunning(IScopeHandle scope){
    ScopeMockHandle self = (ScopeMockHandle) scope->handle;

    self->isRunningHasBeenCalled = true;
}

static void scopePoll(IScopeHandle scope){
    ScopeMockHandle self = (ScopeMockHandle) scope->handle;

    self->scopePollHasBeenCalled = true;
}

static void scopeClear(IScopeHandle scope){
    ScopeMockHandle self = (ScopeMockHandle) scope->handle;

    self->scopeClearHasBeenCalled = true;
}

static void scopeSetTimeIncrement(IScopeHandle scope, uint32_t timeIncrement){
    ScopeMockHandle self = (ScopeMockHandle) scope->handle;

    self->scopeSetTimeIncrementHasBeenCalled = true;
}

static uint32_t getTimeIncrement(IScopeHandle scope){
    ScopeMockHandle self = (ScopeMockHandle) scope->handle;

    self->getTimeIncrementHasBeenCalled = true;
}

static size_t getAmountOfChannels(IScopeHandle scope){
    ScopeMockHandle self = (ScopeMockHandle) scope->handle;

    self->getAmountOfChannelsHasBeenCalled = true;

    return self->amountOfChannels;
}

static IIntStreamHandle getTimestamp(IScopeHandle scope){
    ScopeMockHandle self = (ScopeMockHandle) scope->handle;

    self->getTimestampHasBeenCalled = true;
}

static void configureTrigger(IScopeHandle scope, TriggerConfiguration conf){
    ScopeMockHandle self = (ScopeMockHandle) scope->handle;

    self->configureTriggerHasBeenCalled = true;
}

static void setChannelRunning(IScopeHandle scope, uint32_t idOfChangedChannel){
    ScopeMockHandle self = (ScopeMockHandle) scope->handle;

    self->setChannelRunningHasBeenCalled = true;
}

static void setChannelStopped(IScopeHandle scope, uint32_t idOfChangedChannel){
    ScopeMockHandle self = (ScopeMockHandle) scope->handle;

    self->setChannelStoppedHasBeenCalled = true;
}

static void configureChannelAddress(IScopeHandle scope, void* address,
                                    uint32_t idOfChangedChannel, DATA_TYPES typeOfAddress){
    ScopeMockHandle self = (ScopeMockHandle) scope->handle;

    self->configureChannelAddressHasBeenCalled = true;
}

static void transmit(IScopeHandle scope){
    ScopeMockHandle self = (ScopeMockHandle) scope->handle;

    self->transmitHasBeenCalled = true;
}

static bool run(IRunnableHandle runnable){
    ScopeMockHandle self = (ScopeMockHandle) runnable->handle;

    self->runHasBeenCalled = true;
}

static TriggeredValues getTriggerData(IScopeHandle scope){
    ScopeMockHandle self = (ScopeMockHandle) scope->handle;

    self->getTriggerDataHasBeenCalled = true;
}

static bool channelHasToBePacked(IScopeHandle scope, uint32_t channelId){
    ScopeMockHandle self = (ScopeMockHandle) scope->handle;

    self->channelHasToBePackedHasBeenCalled = true;
}

static FloatRingBufferHandle getChannelBuffer(IScopeHandle scope, uint32_t channelId){
    ScopeMockHandle self = (ScopeMockHandle) scope->handle;

    self->getChannelBufferHasBeenCalled = true;
}


/******************************************************************************
 Public functions
******************************************************************************/
ScopeMockHandle ScopeMock_create(size_t amountOfChannels){

    ScopeMockHandle self = malloc(sizeof(ScopeMockPrivateData));
    assert(self);

    self->scope.handle = self;
    self->scope.poll = &scopePoll;
    self->scope.setTimeIncrement = &scopeSetTimeIncrement;
    self->scope.getTimeIncrement = &getTimeIncrement;
    self->scope.getTimestamp = &getTimestamp;
    self->scope.clear = &scopeClear;
    self->scope.getAmountOfChannels = &getAmountOfChannels;
    self->scope.setChannelStopped = &setChannelStopped;
    self->scope.setChannelRunning = &setChannelRunning;
    self->scope.configureChannelAddress = &configureChannelAddress;
    self->scope.configureTrigger = &configureTrigger;
    self->scope.getTriggerData = &getTriggerData;
    self->scope.channelHasToBePacked = &channelHasToBePacked;
    self->scope.getChannelBuffer = &getChannelBuffer;
    self->scope.transmit = &transmit;
    self->scope.isRunning = &isRunning;
    self->scope.isReadyToRun = &isReadyToRun;

    self->runnable.handle = self;
    self->runnable.run = &run;

    self->amountOfChannels = amountOfChannels;


    self->isReadyToRunHasBeenCalled = false;
    self->isRunningHasBeenCalled = false;
    self->scopePollHasBeenCalled = false;
    self->scopeClearHasBeenCalled = false;
    self->scopeSetTimeIncrementHasBeenCalled = false;
    self->getTimeIncrementHasBeenCalled = false;
    self->getAmountOfChannelsHasBeenCalled = false;
    self->getTimestampHasBeenCalled = false;
    self->configureTriggerHasBeenCalled = false;
    self->setChannelRunningHasBeenCalled = false;
    self->setChannelStoppedHasBeenCalled = false;
    self->configureChannelAddressHasBeenCalled = false;
    self->transmitHasBeenCalled = false;
    self->runHasBeenCalled = false;
    self->getTriggerDataHasBeenCalled = false;
    self->channelHasToBePackedHasBeenCalled = false;
    self->getChannelBufferHasBeenCalled = false;

    return self;
}

void ScopeMock_attachPackObserver(ScopeMockHandle self, IObserverHandle observer){
    self->observer = observer;
}

void ScopeMock_destroy(ScopeMockHandle self){

    free(self);
    self = NULL;
}


IRunnableHandle ScopeMock_getIRunnable(ScopeMockHandle self){
    return &self->runnable;
}

IScopeHandle ScopeMock_getIScope(ScopeMockHandle self){
    return &self->scope;
}
