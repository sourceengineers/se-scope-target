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
#include <Scope/Control/IUnpacker.h>
#include <Scope/Control/Command/ICommand.h>
#include <Scope/Control/CommandParserDispatcher.h>
#include <Scope/GeneralPurpose/IRxTxRunnable.h>
#include <Scope/Core/IScope.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __ControllerPrivateData* ControllerHandle;

/******************************************************************************
 Public functions
******************************************************************************/
ControllerHandle Controller_create(IScopeHandle scope, IPackerHandle packer, IUnpackerHandle unpacker);

IRxTxRunnableHandle Controller_getRxTxRunnable(ControllerHandle self);

void Controller_destroy(ControllerHandle self);

#endif


