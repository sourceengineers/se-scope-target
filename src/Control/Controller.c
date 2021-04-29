/*!****************************************************************************************************************************************
 * @file         Controller.c
 *
 * @copyright    Copyright (c) 2021 by Source Engineers GmbH. All Rights Reserved.
 *
 * @license {    This file is part of se-scope-target.
 *
 *               se-scope-target is free software; you can redistribute it and/or
 *               modify it under the terms of the GPLv3 General Public License Version 3
 *               as published by the Free Software Foundation.
 *
 *               se-scope-target is distributed in the hope that it will be useful,
 *               but WITHOUT ANY WARRANTY; without even the implied warranty of
 *               MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *               GNU General Public License for more details.
 *
 *               You should have received a copy of the GPLv3 General Public License Version 3
 *               along with se-scope-target.  If not, see <http://www.gnu.org/licenses/>.
 *
 *               In closed source or commercial projects, GPLv3 General Public License Version 3
 *               is not valid. In this case the commercial license received with the purchase
 *               is applied (See SeScopeLicense.pdf).
 *               Please contact us at scope@sourceengineers.com for a commercial license.
 * }
 *
 * @authors      Samuel Schuepbach <samuel.schuepbach@sourceengineers.com>
 *
 *****************************************************************************************************************************************/

#include <se-lib-c/util/runnable/IRunnable.h>
#include <se-lib-c/util/observer/IObserver.h>

#include "Scope/Control/CommandPackParserDispatcher.h"
#include "Scope/Control/Controller.h"
#include "Scope/Control/ICommand.h"
#include "Scope/Control/CommandParserDispatcher.h"
#include "Scope/Control/IPacker.h"
#include "Scope/Control/IUnpacker.h"
#include "Scope/Core/IScope.h"
#include "Scope/Core/ScopeTypes.h"
#include "Scope/Control/ParserDefinitions.h"

#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __ControllerPrivateData {
    IRunnable rxRunnable;
    IRunnable txRunnable;
    IScopeHandle scope;
    IUnpackerHandle unpacker;
    IPackerHandle packer;
    CommandParserDispatcherHandle commandParserDispatcher;
    CommandPackParserDispatcherHandle commandPackParserDispatcher;
    IByteStreamHandle logByteStream;

    IObserver commandObserver;
    IObserver commandPackObserver;

    IObserverHandle packObserver;

    MessageType packCommandPending;

    MessageType commandPending;

	uint8_t pendingToPack;

    uint16_t numberOfPackedLogmessagesSent;

} ControllerPrivateData;

static bool runRx(IRunnableHandle runnable);

static bool runTx(IRunnableHandle runnable);

static void commandPackUpdate(IObserverHandle observer, void *state);

static void commandUpdate(IObserverHandle observer, void *state);

/******************************************************************************
 Private functions
******************************************************************************/
static bool runRx(IRunnableHandle runnable) {
    ControllerHandle self = (ControllerHandle) runnable->handle;

    if (self->commandPending == SE_NONE) {
        return false;
    }

    if (self->commandPending == SE_NAK) {
        self->commandPackObserver.update(&self->commandPackObserver, &self->commandPending);
        self->commandPending = SE_NONE;
        return true;
    }

    ICommandHandle command = CommandParserDispatcher_run(self->commandParserDispatcher, self->commandPending);
    if(command != NULL){
        command->run(command);
    }

    self->commandPending = SE_NONE;

    return true;
}


static bool runTx(IRunnableHandle runnable) {
    ControllerHandle self = (ControllerHandle) runnable->handle;
    ICommandHandle packCommand = NULL;

    if (self->packer->isReady(self->packer) == false){
        return false;
    }

	if (self->packCommandPending == SE_NONE) {
		// no command pending, log ready
	   if(self->logByteStream->byteIsReady(self->logByteStream)){
		   MessageType typeToPack = SC_LOG;
		   self->commandPackObserver.update(&self->commandPackObserver, &typeToPack);
		   self->numberOfPackedLogmessagesSent++;
		}
	   else{
		   return true;
	   }
    }

    //another command is pending. Prioritize over log
	else if(self->packCommandPending == SE_ACK || self->packCommandPending == SE_NAK ||
			self->packCommandPending == SC_ANNOUNCE || self->packCommandPending == SC_DETECT ||
			self->packCommandPending == SC_STREAM)
	{
		//important command, skip to packing
    }
	else	 //SC_DATA or SC_LOG is available
    {
    	// data ready, nothing to log
    	if(self->packCommandPending == SC_DATA && !self->logByteStream->byteIsReady(self->logByteStream) && !(self->packCommandPending==SC_LOG))
    	{
    		self->numberOfPackedLogmessagesSent = 0;
    	}
    	//log available, but 10 log Messages sent. Send Data.
    	else if(self->numberOfPackedLogmessagesSent > 10u)
    	{
    		self->packCommandPending = SC_DATA;
    		self->numberOfPackedLogmessagesSent = 0;
    	}
    	else	// send the log, increase the counter
    	{
 		   MessageType typeToPack = SC_LOG;
 		   self->commandPackObserver.update(&self->commandPackObserver, &typeToPack);
 		   self->numberOfPackedLogmessagesSent++;
    	}
    }

	packCommand = CommandPackParserDispatcher_run(self->commandPackParserDispatcher, self->packCommandPending);
	if(packCommand != NULL){
		packCommand->run(packCommand);
	}
	self->packObserver->update(self->packObserver, &self->packCommandPending);
	self->packCommandPending = SE_NONE;
	return true;
}


static void commandPackUpdate(IObserverHandle observer, void *state) {
    ControllerHandle self = (ControllerHandle) observer->handle;
    if(self->packCommandPending != SE_NONE){
        return;
    }
    self->packCommandPending = *(MessageType*) state;
}

static void commandUpdate(IObserverHandle observer, void *state) {
    ControllerHandle self = (ControllerHandle) observer->handle;
    self->commandPending = *(MessageType*) state;
}

/******************************************************************************
 Public functions
******************************************************************************/
ControllerHandle Controller_create(IScopeHandle scope, IPackerHandle packer, IUnpackerHandle unpacker,
                                   AnnounceStorageHandle announceStorage, IByteStreamHandle logByteStream) {

    ControllerHandle self = malloc(sizeof(ControllerPrivateData));
    assert(self);

    self->rxRunnable.handle = self;
    self->txRunnable.handle = self;
    self->rxRunnable.run = &runRx;
    self->txRunnable.run = &runTx;
    self->unpacker = unpacker;
    self->packer = packer;
    self->scope = scope;

    self->commandPackObserver.handle = self;
    self->commandObserver.handle = self;

    self->commandPackObserver.update = &commandPackUpdate;
    self->commandObserver.update = &commandUpdate;

    self->logByteStream = logByteStream;

    self->commandParserDispatcher = CommandParserDispatcher_create(scope, &self->commandPackObserver, unpacker);

    self->commandPackParserDispatcher = CommandPackParserDispatcher_create(scope, announceStorage, packer, logByteStream);


    self->packCommandPending = SE_NONE;
    self->commandPending = SE_NONE;

    self->numberOfPackedLogmessagesSent = 0;

    return self;
}


void Controller_attachPackObserver(ControllerHandle self, IObserverHandle observer) {
    self->packObserver = observer;
}

bool Controller_commandPending(ControllerHandle self){
    return self->commandPending;
}

IObserverHandle Controller_getCommandObserver(ControllerHandle self) {
    return &self->commandObserver;
}

IObserverHandle Controller_getCommandPackObserver(ControllerHandle self) {
    return &self->commandPackObserver;
}

IRunnableHandle Controller_getRxRunnable(ControllerHandle self) {
    return &self->rxRunnable;
}

IRunnableHandle Controller_getTxRunnable(ControllerHandle self) {
    return &self->txRunnable;
}

void Controller_destroy(ControllerHandle self) {
    CommandParserDispatcher_destroy(self->commandParserDispatcher);
    CommandPackParserDispatcher_destroy(self->commandPackParserDispatcher);

    free(self);
    self = NULL;
}
