/*!*****************************************************************************
 * @file         Controller.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 ******************************************************************************/

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Scope/Control/IPacker.h>
#include <Scope/GeneralPurpose/IRunnable.h>
#include <Scope/Control/CommandParserDispatcher.h>
#include <Scope/GeneralPurpose/IObserver.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __ControllerPrivateData* ControllerHandle;

/******************************************************************************
 Public functions
******************************************************************************/
ControllerHandle Controller_create(IScopeHandle scope, IPackerHandle packer, IUnpackerHandle unpacker);

IRunnableHandle Controller_getRxRunnable(ControllerHandle self);

IRunnableHandle Controller_getTxRunnable(ControllerHandle self);

void Controller_attachPackObserver(ControllerHandle self, IObserverHandle observer);

IObserverHandle Controller_getCommandObserver(ControllerHandle self);

IObserverHandle Controller_getCommandPackObserver(ControllerHandle self);

void Controller_destroy(ControllerHandle self);

#endif


