/*!*****************************************************************************
 * @file         Trigger.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the Trigger module.
 *               The trigger can have three different configurations:
 *               Continious: The trigger will not trigger.
 *               Normal: The trigger triggers on a given edge and value
 *               OneShot: Same as the normal mode, but stops the Trigger
 *
 *               The state pattern has been implemented for every configuration.
 *
 *               The trigger can be configured through commands.
 *
 *
 ******************************************************************************/

#ifndef TRIGGER_H_
#define TRIGGER_H_

#include <Scope/Core/Channel.h>
#include <Scope/Core/ScopeTypes.h>
#include <Scope/Core/Timestamper.h>
#include <Scope/GeneralPurpose/DataTypes.h>

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __TriggerPrivateData* TriggerHandle;

/******************************************************************************
Public functions 
******************************************************************************/
/**
 * Creates a new trigger
 * @param channels Reference to all channels
 * @param amountOfChannels Max amount of channel
 * @param channelCapacity Max about of the channel capacity
 * @param timestamper Reference to the timestamper
 * @return
 */
TriggerHandle Trigger_create(ChannelHandle* channels, size_t amountOfChannels, \
                                size_t channelCapacity, TimestamperHandle timestamper);

/**
 * Configures the trigger
 * @param self
 * @param conf
 * @return Return false if the input data was not valid
 */
bool Trigger_configure(TriggerHandle self, TriggerConfiguration conf);

/**
 * Returns the timestamp at which the trigger triggered
 * @param self
 * @return
 */
uint32_t Trigger_getTriggerIndex(TriggerHandle self);

/**
 * Returns if the trigger found a matching event or not
 * @param self
 * @return True if the trigger found a event, false if not
 */
bool Trigger_isTriggered(TriggerHandle self);

/**
 * Returns the index of the channel the trigger is currently monitoring
 * @param self
 * @return
 */
uint32_t Trigger_getChannelId(TriggerHandle self);

/**
 * Executes the trigger
 * @param self
 * @return
 */
bool Trigger_run(TriggerHandle self);

/**
 * Activates the trigger
 * @param self
 */
void Trigger_activate(TriggerHandle self);

/**
 * Deactivates the trigger
 * @param self
 */
void Trigger_deactivate(TriggerHandle self);

/**
 * Resets the trigger
 * @param self
 */
void Trigger_clear(TriggerHandle self);

/**
 * Deconstructor
 * @param self
 */
void Trigger_destroy(TriggerHandle self);

#endif
