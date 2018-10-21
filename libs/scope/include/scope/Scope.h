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

#include <Command/CommandFactory.h>
#include <Communication/Receiver.h>

#include <GeneralPurpose/ByteStream.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __ScopePrivateData* ScopeHandle;

typedef enum {TIMESTAMP_AUTOMATIC, TIMESTAMP_MANUAL} TIMESTAMPING_MODE;
/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the channel */
ScopeHandle Scope_create(size_t channelSize, size_t numberOfChannels, COM_TYPE comType,
                         TIMESTAMPING_MODE timestampingMode,
                         ScopeTransmitCallback transmitCallback);

/* Deconstructor: Deletes the instance of the channel */
void Scope_destroy(ScopeHandle self);

/* Passes data to the scope which has to be parsed. The data has to be in the form of the specified protocol, or will
 * be rejected.
 * After parsing, the commands will be executed */
void Scope_command(ScopeHandle self);

/* Returns the input stream which feeds data into the Receiver */
IByteStreamHandle Scope_getInputStream(ScopeHandle self);

/* Returns the output stream, with data fed by the sender */
IByteStreamHandle Scope_getOutputStream(ScopeHandle self);

/* Packs all the necessary data into a package ready to be sent */
void Scope_packMessage(ScopeHandle self);

/* Polls data from all channels */
void Scope_poll(ScopeHandle self, uint32_t timeStamp);

#endif
