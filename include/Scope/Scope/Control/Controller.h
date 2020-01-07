/*!*****************************************************************************
 * @file         Controller.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Layer which controls the commands used to pack the scope data
 *               and passed commands from the lower layers into the scop.e
 *
 ******************************************************************************/

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <se-lib-c/util/runnable/IRunnable.h>
#include <se-lib-c/util/observer/IObserver.h>

#include "Scope/Control/AnnounceStorage.h"
#include "Scope/Control/CommandParserDispatcher.h"
#include "Scope/Control/IPacker.h"
#include "Scope/Control/IUnpacker.h"
#include "Scope/Core/IScope.h"

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __ControllerPrivateData* ControllerHandle;

/******************************************************************************
 Public functions
******************************************************************************/
/**
 * Constructor
 * @param scope
 * @param packer
 * @param unpacker
 * @param announceStorage
 * @return
 */
ControllerHandle Controller_create(IScopeHandle scope, IPackerHandle packer, IUnpackerHandle unpacker,
                                   AnnounceStorageHandle announceStorage);

/**
 * Returns the runnable of the rx controller
 * @param self
 * @return
 */
IRunnableHandle Controller_getRxRunnable(ControllerHandle self);

/**
 * Returns the runnable of the tx controller
 * @param self
 * @return
 */
IRunnableHandle Controller_getTxRunnable(ControllerHandle self);

/**
 * Returns if a command is pending to be interpreted
 * @param self
 * @return true if a command is pending. False if not
 */
bool Controller_commandPending(ControllerHandle self);

/**
 * Attach the observer to the controller
 * @param self
 * @param observer
 */
void Controller_attachPackObserver(ControllerHandle self, IObserverHandle observer);

/**
 * Returns the observer of the controller
 * @param self
 * @return
 */
IObserverHandle Controller_getCommandObserver(ControllerHandle self);

/**
 * Returns the observer to the pack controller
 * @param self
 * @return
 */
IObserverHandle Controller_getCommandPackObserver(ControllerHandle self);

/**
 * Deconstructor
 * @param self
 */
void Controller_destroy(ControllerHandle self);

#endif


