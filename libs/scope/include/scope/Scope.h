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
#include <Communication/Reciever.h>

/* Defines class */
typedef struct __ScopePrivateData* ScopeHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instanze of the channel */
ScopeHandle Scope_create(size_t channelSize, size_t numberOfChannels, size_t communicationBufferSize, COM_TYPE comType);

/* Deconstructor: Deletes the instanze of the channel */
void Scope_destroy(ScopeHandle self);

/* Passes data to the scope which has to be parsed. The data has to be in the form of the specified protocol, or will
 * be rejected.
 * After parsing, the commands will be executed */
void Scope_command(ScopeHandle self, const char* data, size_t dataLength);


/* Polls data from all channels */
void Scope_poll(ScopeHandle self);

#endif
