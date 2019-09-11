/*!*****************************************************************************
 * @file         Scope.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the Scope.
 * 
 ******************************************************************************/

#ifndef SCOPE_H_
#define SCOPE_H_

#include <Scope/Control/AnnounceStorage.h>
#include <Scope/Core/Channel.h>
#include <Scope/Core/IScope.h>
#include <Scope/Core/ScopeTypes.h>
#include <Scope/Core/Trigger.h>
#include <Scope/GeneralPurpose/DataTypes.h>
#include <Scope/GeneralPurpose/IObserver.h>
#include <Scope/GeneralPurpose/IRunnable.h>

#include <stdint.h>
#include <stddef.h>
#include <stdint.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __ScopePrivateData* ScopeHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/**
 * Constructor
 * @param channelSize Amount of space each channel buffer has
 * @param amountOfChannels Amount of channels that should be used
 * @param referenceTimestamp Reference to an externally incremented timestamp. Used for the Timestamper
 * @return
 */
ScopeHandle Scope_create(size_t channelSize, size_t amountOfChannels, uint32_t* referenceTimestamp);

/**
 * Commands the scope to transmit its data
 * @param self
 */
void Scope_transmit(ScopeHandle self);

/**
 * Tells the scope to poll data in the active channels
 * @param self
 * @return Returns if the scope is done running
 */
bool Scope_poll(ScopeHandle self);

/**
 * Configurates the channels with id == channelId toa specific address.
 * @param self
 * @param channelId If channelId > max amount of channels, the command gets rejected.
 * @param pollAddress Address which the channel should poll from
 * @param type Type of the data stored in pollAddress
 */
void Scope_configureChannel(ScopeHandle self, size_t channelId, void* pollAddress, DATA_TYPES type);

/**
 * Configurates the trigger with the gives values
 * @param self
 * @param level Can be any float value
 * @param edge This can be either TRIGGER_EDGE_NEGATIVE or TRIGGER_EDGE_POSITIVE
 * @param mode Chooses the trigger mode. Can be either: TRIGGER_CONTINUOUS, TRIGGER_NORMAL or TRIGGER_ONESHOT
 * @param channelId The id which should be monitored by the trigger. If channelId doesn't match any channel,
 *  this command gets rejected
 */
void Scope_configureTrigger(ScopeHandle self, float level, int edge, TRIGGER_MODE mode, uint32_t channelId);

/**
 * Sets the time increment between poll events.
 * @param self
 * @param timstampIncrement
 */
void Scope_configureTimestampIncrement(ScopeHandle self, uint32_t timstampIncrement);

/**
 * Starts the channel with its id matching idOfChangedChannel
 * @param self
 * @param channelId If channelId > max amount of channels, the command gets rejected.
 */
void Scope_setChannelRunning(ScopeHandle self, uint32_t channelId);

/**
 * Stopps the channel with its id matching idOfChangedChannel
 * @param self
 * @param channelId If channelId > max amount of channels, the command gets rejected.
 */
void Scope_setChannelStopped(ScopeHandle self, uint32_t channelId);

/**
 * Clears the timestamp stream as well as all channels
 * @param self
 */
void Scope_clear(ScopeHandle self);

/**
 * Attatches the observer which has to be called to execute a pack event
 * @param self
 * @param observer
 */
void Scope_attachPackObserver(ScopeHandle self, IObserverHandle observer);

/**
 * Returns the runnable interface
 * @param self
 * @return
 */
IRunnableHandle Scope_getIRunnable(ScopeHandle self);

/**
 * Returns the IScope interface
 * @param self
 * @return
 */
IScopeHandle Scope_getIScope(ScopeHandle self);

/**
 * Deconstructor
 * @param self
 */
void Scope_destroy(ScopeHandle self);

#endif

