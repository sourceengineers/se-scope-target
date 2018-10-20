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
 
#ifndef Sender_H_
#define Sender_H_

#include <unistd.h>
#include <Communication/IPacker.h>
#include <Communication/ComTypes.h>
#include <GeneralPurpose/IByteStream.h>
#include <Scope/Channel.h>
#include <Scope/Trigger.h>
#include <Scope/Scope.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __SenderPrivateData* SenderHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the Sender */
SenderHandle Sender_create(IPackerHandle packer, ChannelHandle* channels, const size_t numberOfChannels,
                           COM_TYPE comType,
                           TriggerHandle trigger,
                           IScopeHandle scope);

/* Prepares a data package with channel and trigger data */
void Sender_pack(SenderHandle self);

/* Transmits a data package */
bool Sender_transmit(SenderHandle self);

/* Deconstructor: Deletes the instance of the Sender */
void Sender_destroy(SenderHandle self);


#endif
