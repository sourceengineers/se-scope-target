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

    static bool isMutexLocked = false;
    bool success = false;

    if (!scope.configMutex || scope.configMutex->lock(scope.configMutex, 0)) {
        if (!scope.scope->isRunning(scope.scope) && scope.scope->isReadyToRun(scope.scope)) {
            scope.dataMutex->lock(scope.dataMutex, 0);
            isMutexLocked = true;
        }

        if (isMutexLocked) {
            if (!scope.runScope->run(scope.runScope)) {
                scope.dataMutex->unlock(scope.dataMutex);
                isMutexLocked = false;
            }
        }
    }
}

void ScopeThreadRunner_runStack(ScopeObject scope) {

    scope.runCommunicationRx->run(scope.runCommunicationRx);
    scope.runUnpacker->run(scope.runUnpacker);

    if (Controller_commandPending(scope.controller)) {
        if (!scope.configMutex || scope.configMutex->lock(scope.configMutex, 10)) {
            scope.runCommandParser->run(scope.runCommandParser);
            scope.configMutex->unlock(scope.configMutex);
        }
    }

    if (scope.dataMutex->lock(scope.configMutex, 10)) {
        scope.runDataAggregator->run(scope.runDataAggregator);
        scope.runPacker->run(scope.runPacker);
        scope.runCommunicationTx->run(scope.runCommunicationTx);
        scope.dataMutex->unlock(scope.dataMutex);
    }
}
