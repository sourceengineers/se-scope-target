/*!*****************************************************************************
 * @file         Sender.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the Sender.
 *
 *               For the moment the sender can only pack trigger and channel
 *               data. More advanced things like flow control and support for
 *               multiple communication methods will be implemented in a later
 *               step.
 * 
 ******************************************************************************/
 
#ifndef SENDER_H_
#define SENDER_H_

#include <unistd.h>
#include <Scope/Communication/IPacker.h>
#include <Scope/Communication/CommunicationCommons.h>
#include <Scope/GeneralPurpose/IByteStream.h>
#include <Scope/Channel.h>
#include <Scope/Trigger.h>
#include <Scope/IScope.h>
#include <Scope/AddressStorage.h>
#include <Scope/GeneralPurpose/DataTypes.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __SenderPrivateData* SenderHandle;

typedef void(*ScopeTransmitCallback)(IByteStreamHandle stream);

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the Sender */
SenderHandle Sender_create(IPackerHandle packer, ChannelHandle* channels, const size_t numberOfChannels,
                           TriggerHandle trigger,
                           IScopeHandle scope,
                           ScopeTransmitCallback transmitCallback,
                           AddressStorageHandle addressStorage);

/* Prepares a data package with channel and trigger data */
void Sender_scopeData(SenderHandle self);

/* Transmits a data package */
bool Sender_transmit(SenderHandle self);

/* Prepares a msgpack packages, containing a ack or nak */
void Sender_flowControl(SenderHandle self, const char* flowControl);

/* Transmits all addresses which are registered to be sent */
void Sender_addressAnnouncement(SenderHandle self);

/* Deconstructor: Deletes the instance of the Sender */
void Sender_destroy(SenderHandle self);


#endif
