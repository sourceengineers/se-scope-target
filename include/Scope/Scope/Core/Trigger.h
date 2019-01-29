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
 *               OneShot: Same as the normal mode, but stopps the Trigger
 *
 *               To configure the trigger, the specific Trigger has to be passed
 *               to the trigger and the trigger mode has to be choosen. 
 *
 *               TODO - OneShot tigger has to be implemented
 *
 ******************************************************************************/
 
#ifndef TRIGGER_H_
#define TRIGGER_H_

#include <Scope/Core/Channel.h>
#include <Scope/GeneralPurpose/DataTypes.h>
#include <Scope/Core/ScopeTypes.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __TriggerPrivateData* TriggerHandle;

/* Typedef for a function pointer, to easier handle the strategies */
typedef bool (*TriggerStrategy)(TriggerHandle self, const uint32_t index);

/******************************************************************************
Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the Trigger */
TriggerHandle Trigger_create(ChannelHandle* channels, size_t amountOfChannels);

/* Deconstructor: Deletes the instance of the Trigger */
void Trigger_destroy(TriggerHandle self);

/* Configures the trigger */
bool Trigger_configure(TriggerHandle self, TriggerConfiguration conf);

/* Returns the triggered index */
uint32_t Trigger_getTriggerIndex(TriggerHandle self);

/* Returns if the trigger is triggered or not */
bool Trigger_isTriggered(TriggerHandle self);

/* Releases the currently triggered value */
void Trigger_release(TriggerHandle self);

/* Returns the id of the channel which is currently active */
uint32_t Trigger_getChannelId(TriggerHandle self);

/* Function which calls the currently chosen trigger strategy */
bool Trigger_run(TriggerHandle self, const uint32_t timstamp);

#endif
