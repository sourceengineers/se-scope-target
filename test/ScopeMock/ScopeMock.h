/*!*****************************************************************************
 * @file         ScopeMock.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach <samuel.schuepbach@sourceengineers.com>
 *
 * @brief        Implementation of the ScopeMock.
 * 
 ******************************************************************************/

#ifndef SCOPEMOCK_H_
#define SCOPEMOCK_H_

#include "Scope/GeneralPurpose/DataTypes.h"

#include <se-lib-c/util/runnable/IRunnable.h>
#include <se-lib-c/util/observer/IObserver.h>
#include "Scope/Core/IScope.h"
#include "Scope/Core/ScopeTypes.h"

#include <stdint.h>
#include <stddef.h>
#include <stdint.h>

typedef struct __ScopeMockPrivateData{
    IRunnable runnable;
    IScope scope;

    IObserverHandle observer;

    bool isReadyToRunHasBeenCalled;
    bool isRunningHasBeenCalled;
    bool scopePollHasBeenCalled;
    bool scopeClearHasBeenCalled;
    bool scopeSetTimeIncrementHasBeenCalled;
    bool getTimeIncrementHasBeenCalled;
    bool getAmountOfChannelsHasBeenCalled;
    bool getTimestampHasBeenCalled;
    bool configureTriggerHasBeenCalled;
    bool setChannelRunningHasBeenCalled;
    bool setChannelStoppedHasBeenCalled;
    bool configureChannelAddressHasBeenCalled;
    bool transmitHasBeenCalled;
    bool runHasBeenCalled;
    bool getTriggerDataHasBeenCalled;
    bool channelHasToBePackedHasBeenCalled;
    bool getChannelBufferHasBeenCalled;

    size_t amountOfChannels;
} ScopeMockPrivateData;

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __ScopeMockPrivateData* ScopeMockHandle;

ScopeMockHandle ScopeMock_create(size_t amountOfChannels);

IScopeHandle ScopeMock_getIScope(ScopeMockHandle self);

void ScopeMock_destroy(ScopeMockHandle self);

#endif

