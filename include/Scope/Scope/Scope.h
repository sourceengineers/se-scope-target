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

#include <Scope/Trigger.h>
#include <Scope/Channel.h>
#include <Scope/IScope.h>

#include <Scope/Command/CommandFactory.h>
#include <Scope/Communication/Receiver.h>

#include <Scope/GeneralPurpose/ByteStream.h>
#include <Scope/GeneralPurpose/DataTypes.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __ScopePrivateData* ScopeHandle;

typedef enum {TIMESTAMP_AUTOMATIC, TIMESTAMP_MANUAL} TIMESTAMPING_MODE;
/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the channel */
ScopeHandle Scope_create(const size_t channelSize,
                         const size_t numberOfChannels,
                         const size_t maxNumberOfAddresses,
                         const COM_TYPE comType,
                         const TIMESTAMPING_MODE timestampingMode,
                         ScopeTransmitCallback transmitCallback);

/* Deconstructor: Deletes the instance of the channel */
void Scope_destroy(ScopeHandle self);

/* Passes data to the scope which has to be parsed. The data has to be in the form of the specified protocol, or will
 * be rejected.
 * After parsing, the commands will be executed */
void Scope_receiveData(ScopeHandle self);

/* Returns the input stream which feeds data into the Receiver */
IByteStreamHandle Scope_getInputStream(ScopeHandle self);

/* Returns the output stream, with data fed by the sender */
IByteStreamHandle Scope_getOutputStream(ScopeHandle self);

/* Packs all the necessary data into a package ready to be sent */
void Scope_transmitData(ScopeHandle self);

/* Polls data from all channels */
void Scope_poll(ScopeHandle self, gemmi_uint timeStamp);

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
void Scope_configureTrigger(ScopeHandle self, const float level, int edge, TRIGGER_MODE mode, gemmi_uint channelId);

/* Sets the timestamp increment
 * If the timestamp increment mode is set to manual, this will not have any effect */
void Scope_configureTimestampIncrement(ScopeHandle self, gemmi_uint timstampIncrement);

/* Sets the channel with the given index to running */
void Scope_setChannelRunning(ScopeHandle self, gemmi_uint channelId);

/* Sets the channel with the given index to stopped */
void Scope_setChannelStopped(ScopeHandle self, gemmi_uint channelId);

/* Sends all configured watch addresses to the host */
void Scope_announceAddresses(ScopeHandle self);

/* Sets a new watch address. Returns if the index exceeds the maximum amount of elements */
void Scope_addAnnounceAddresses(ScopeHandle self, const char* name, const void* address,
                             const DATA_TYPES type,
                             const gemmi_uint addressId);

/* Clears the data in the channels, as well as the timestamp buffer */
void Scope_clear(ScopeHandle self);


#endif
