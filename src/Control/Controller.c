/*!****************************************************************************************************************************************
 * @file         Controller.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
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


//		/* OLD RUNTX */
static bool runTx(IRunnableHandle runnable) {
    ControllerHandle self = (ControllerHandle) runnable->handle;


    if (self->packer->isReady(self->packer) == false){
        return false;
    }

    // TODO smarter handling of this

    if (self->packCommandPending == SE_NONE) {	// only send the log, if no other command is pending
	   if(self->logByteStream->byteIsReady(self->logByteStream)){
			Scope_log(self->scope->handle);	//TODO replace this with the following:
//		    MessageType typeToPack = SC_LOG;
//		    self->packObserver->update(self->packObserver, &typeToPack);
		}
	   else{
	        return false;
	   }
    }

    // TODO wenn Commands verfügbar sind, schauen, was zuletzt gemacht wurde und dann das andere machen
    //		also zuletzt geloggt -> plotten, zuletzt geplottet -> loggen. So kommt beides an die Reihe


    ICommandHandle packCommand;

    packCommand = CommandPackParserDispatcher_run(self->commandPackParserDispatcher, self->packCommandPending);
    if(packCommand != NULL){
        packCommand->run(packCommand);
    }

    self->packObserver->update(self->packObserver, &self->packCommandPending);
	self->packCommandPending = SE_NONE;

    return true;
}



//		/* NEW RUNTX. Not working yet */
//static bool runTx(IRunnableHandle runnable) {
//    ControllerHandle self = (ControllerHandle) runnable->handle;
//    ICommandHandle packCommand = NULL;
//    // wait here until packer is no more busy and until byte transport layer has emptied the output buffer
//    // TODO is it possible that packCommandPending is overwritten and therefore messages are lost?
//    if (self->packer->isReady(self->packer) == false){
//        return false;
//    }
//
//    // if isImageTransferCompleted
//    //    startLog Transfer
//    // if isLogTransferCompleted
//
//    // TODO smarter handling of this
//	if (self->packCommandPending == SE_NONE) {
//	   if(self->logByteStream->byteIsReady(self->logByteStream)){
//		   // if no command is pending, but there is something in the LogByteStream, send this.
////		   packCommand = CommandPackParserDispatcher_run(self->commandPackParserDispatcher, SC_LOG);
//		   Scope_log(self->scope->handle);	//TODO replace this
//		   self->numberOfPackedLogmessagesSent++;
//		}
//    }
//    //another, probably importand command is pending, do this.
//    else if(self->packCommandPending != SC_DATA)
//    {
//        packCommand = CommandPackParserDispatcher_run(self->commandPackParserDispatcher, self->packCommandPending);
//        self->packCommandPending = SE_NONE;
//    }
//    //SC_DATA is available
//    else
//    {
//    	// nothing to log, just send SC_DATA
//    	if(!self->logByteStream->byteIsReady(self->logByteStream))
//    	{
//    		packCommand = CommandPackParserDispatcher_run(self->commandPackParserDispatcher, self->packCommandPending);
//            self->packCommandPending = SE_NONE;
//    	}
//    	// if many logs have been sent, send the data and reset numberOfPackedLogmessagesSent
//    	else if(self->numberOfPackedLogmessagesSent > 10u)
//    	{
//    		packCommand = CommandPackParserDispatcher_run(self->commandPackParserDispatcher, self->packCommandPending);
//            self->packCommandPending = SE_NONE;
//    		self->numberOfPackedLogmessagesSent = 0;
//    	}
//    	else
//    	{
//    	// send the log, inc the counter
//			packCommand = CommandPackParserDispatcher_run(self->commandPackParserDispatcher, SC_LOG);
//			self->numberOfPackedLogmessagesSent++;
//    	}
//    }
//
//
//
//	//TODO wann muss self->packCommandPending = SE_NONE; gesetzt werden?
//
//    // TODO wird scope zu krass bevorzugt? Macht es sinn nach einem scope-bild einige kByte log messages zu versendne bevor das
//    //      scope wider dran kommt?
//
//    // TODO wenn Commands verfügbar sind, schauen, was zuletzt gemacht wurde und dann das andere machen
//    //		also zuletzt geloggt -> plotten, zuletzt geplottet -> loggen. So kommt beides an die Reihe
//
//
//
//
//    if(packCommand != NULL){
//        packCommand->run(packCommand);
//        self->packCommandPending = SE_NONE;
//    }
//
//    self->packObserver->update(self->packObserver, &self->packCommandPending);
//
//    // TODO reset this flag only after output buffer is empty?
////	self->packCommandPending = SE_NONE;
//
//    return true;
//}

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
