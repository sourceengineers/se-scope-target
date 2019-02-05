/*!*****************************************************************************
 * @file         Communicator.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 ******************************************************************************/

#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <Scope/GeneralPurpose/IRunnable.h>
#include <Scope/Communication/ICommunicator.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommunicatorPrivateData* CommunicatorHandle;

/******************************************************************************
 Public functions
******************************************************************************/
CommunicatorHandle Communicator_create(ICommunicatorHandle communicator);

IRunnableHandle Communicator_getRxRunnable(CommunicatorHandle self);

IRunnableHandle Communicator_getTxRunnable(CommunicatorHandle self);

void Communicator_destroy(CommunicatorHandle self);

#endif


