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

#include <Scope/GeneralPurpose/IRunnable.h>
#include <Scope/Core/Trigger.h>
#include <Scope/Core/Channel.h>
#include <Scope/Core/IScope.h>
#include <Scope/Core/AddressStorage.h>
#include <Scope/GeneralPurpose/DataTypes.h>
#include <Scope/Core/ScopeTypes.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __ScopePrivateData* ScopeHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the channel */
ScopeHandle Scope_create(size_t channelSize,
                         size_t amountOfChannels,
                         AddressStorageHandle addressStorage,
                         uint32_t* referenceTimestamp);

/* Deconstructor: Deletes the instance of the channel */
void Scope_destroy(ScopeHandle self);

/* Tells the scope to transmit the data in its channels */
void Scope_transmit(ScopeHandle self);

/* Tells the scope to transmit the announce addresses */
void Scope_announce(ScopeHandle self);

/* Passes data to the scope which has to be parsed. The data has to be in the form of the specified protocol, or will
 * be rejected.
 * After parsing, the commands will be executed */
void Scope_receiveData(ScopeHandle self);

/* Packs all the necessary data into a package ready to be sent */
//void Scope_transmitData(ScopeHandle self);

/* Polls data from all channels */
void Scope_poll(ScopeHandle self);

/* Configures the channel with the given id, with the wanted address */
/* If the id exceeds the maximum amount of channels, the function will return without doing anything */
void Scope_configureChannel(ScopeHandle self, const size_t channelId, void* pollAddress, DATA_TYPES type);

/* Configurates the trigger with the gives values
 * level: can be any float value
 * edge: This can be either TRIGGER_EDGE_NEGATIVE or TRIGGER_EDGE_POSITIVE
 * mode: Chooses the trigger mode. Can be either: TRIGGER_CONTINUOUS, TRIGGER_NORMAL or TRIGGER_ONESHOT
 * channelId: The id which should be watched by the trigger
 *
 * If the id exceeds the maximum amount of channels, the function will return without doing anything
 * */
void Scope_configureTrigger(ScopeHandle self, const float level, int edge, TRIGGER_MODE mode, uint32_t channelId);

/* Sets the timestamp increment
 * If the timestamp increment mode is set to manual, this will not have any effect */
void Scope_configureTimestampIncrement(ScopeHandle self, uint32_t timstampIncrement);

/* Sets the channel with the given index to running */
void Scope_setChannelRunning(ScopeHandle self, uint32_t channelId);

/* Sets the channel with the given index to stopped */
void Scope_setChannelStopped(ScopeHandle self, uint32_t channelId);

/* Sends all configured watch addresses to the host */
//void Scope_announceAddresses(ScopeHandle self);

/* Sets a new watch address. Returns if the index exceeds the maximum amount of elements */
void Scope_addAnnounceAddresses(ScopeHandle self, const char* name, const void* address,
                             const DATA_TYPES type,
                             const uint32_t addressId);

/* Clears the data in the channels, as well as the timestamp buffer */
void Scope_clear(ScopeHandle self);

/* Returns the Runnable of the scope */
IRunnableHandle Scope_getIRunnable(ScopeHandle self);

/* Returns the IScope interface */
IScopeHandle Scope_getIScope(ScopeHandle self);

#endif

