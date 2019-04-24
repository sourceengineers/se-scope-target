/*!****************************************************************************************************************************************
 * @file         ScopeThreadRunner.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Builders/ScopeThreadRunner.h>
#include <Scope/Core/Scope.h>
#include <Scope/Control/Controller.h>
#include <Scope/Serialisation/Serializer.h>
#include <Scope/GeneralPurpose/IMutex.h>

/******************************************************************************
 Public functions
******************************************************************************/
void ScopeThreadRunner_runScope(ScopeObject scope) {

   // static bool isMutexLocked = false;

    if(scope.configMutex == NULL){
        return;
    }

    /* Lock the config mutex so the stack thread doesn't overwrite the scope config while running */
    if(scope.configMutex->lock(scope.configMutex, 0) == false){
        return;
    }

    /* Lock the data mutex if the scope is running or about to run */
    if (scope.scope->isRunning(scope.scope) || scope.scope->isReadyToRun(scope.scope)) {
				/* Try to lock the data mutex. If the mutex cannot be aquired, the sope will not be run.
						This might happen if the 	scope is being reconfigured */
        
			if(scope.dataMutex->lock(scope.dataMutex, 0) == true){
					scope.runScope->run(scope.runScope);
					scope.dataMutex->unlock(scope.dataMutex);
				}
    }
		
    /* Release the config mutex */
    scope.configMutex->unlock(scope.configMutex);
}

void ScopeThreadRunner_runStack(ScopeObject scope) {

    if(scope.configMutex == NULL){
        return;
    }

    /* Process all pending data. This can be done no matter what the state of the
     * mutexes is, since it doesn't interact with the scope */
    scope.runCommunicationRx->run(scope.runCommunicationRx);
    scope.runUnpacker->run(scope.runUnpacker);

    /* If data is pending, the config mutex has to be locked, so the scope can be reconfigured */
    if (Controller_commandPending(scope.controller)) {
        if (scope.configMutex->lock(scope.configMutex, 0) == true) {
            /* Reconfigure the scope */
            scope.runCommandParser->run(scope.runCommandParser);
            scope.configMutex->unlock(scope.configMutex);
        }
    }

    /* If data is pending to be transmitted, the dataMutex has to be locked, so no new data is put into the
     * output streams while data is still being processed.*/
    if (scope.dataMutex->lock(scope.dataMutex, 0)) {
        scope.runDataAggregator->run(scope.runDataAggregator);
        scope.runPacker->run(scope.runPacker);
        scope.runCommunicationTx->run(scope.runCommunicationTx);
        scope.dataMutex->unlock(scope.dataMutex);
    }
}
