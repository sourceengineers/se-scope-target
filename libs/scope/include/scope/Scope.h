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

/* Defines class */
typedef struct __ScopePrivateData* ScopeHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instanze of the channel */
ScopeHandle Scope_create(size_t channelSize, size_t numberOfChannels);

/* Deconstructor: Deletes the instanze of the channel */
void Scope_destroy(ScopeHandle self);

/* Polls data from all channels */
void Scope_poll(ScopeHandle self);

#endif
