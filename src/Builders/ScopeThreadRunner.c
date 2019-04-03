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
void ScopeThreadRunner_runScope(ScopeObject scope){

    static bool isMutexLocked = false;
    bool success = false;

    if(!scope.configMutex || scope.configMutex->lock(scope.configMutex)){
       if(!scope.scope->isRunning(scope.scope) && scope.scope->isReadyToRun(scope.scope)){
           scope.dataMutex->lock(scope.dataMutex);
           isMutexLocked = true;
       }

       if(isMutexLocked){
           if(!scope.runScope()){
               scope.dataMutex->unlock(scope.dataMutex);
               isMutexLocked = false;
           }
       }
    }
}

void ScopeThreadRunner_runStack(ScopeObject scope){

    scope.runCommunicationRx->run(scope.runCommunicationRx);
    scope.runUnpacker->run(scope.runUnpacker);
    scope.runCommandParser->run(scope.runCommandParser);
    scope.runScope->run(scope.runScope);
    scope.runDataAggregator->run(scope.runDataAggregator);
    scope.runPacker->run(scope.runPacker);
    scope.runCommunicationTx->run(scope.runCommunicationTx);


    /*
     *   46   │     scope.runCommunicationRx->run(scope.runCommunicationRx);
  47   │     scope.runUnpacker->run(scope.runUnpacker
  48   │
  49   │     if(scope.unpacker->hasData(scope.runUnpacker))
  50   │     {
  51   │         if(!configMutex || configMutex->lock(configMutex->handle,10))
  52   │         {
  53   │             scope.runCommandParser->run(scope.runCommandParser);
  54   │             configMutex->unlock()
  55   │         }
  56   │     }
  57   │
  58   │     if(dataMutex->lock(configMutex->handle))
  59   │     {
  60   │         scope.runDataAggregator->run(scope.runDataAggregator);
  61   │         scope.runPacker->run(scope.runPacker);
  62   │         scope.runCommunicationTx->run(scope.runCommunicationTx);
  63   │         dataMutex->unlock()
  64   │     }
ex
     */
}
