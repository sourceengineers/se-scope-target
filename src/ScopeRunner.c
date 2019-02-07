/*!****************************************************************************************************************************************
 * @file         ScopeRunner.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/ScopeRunner.h>
#include <Scope/Core/Scope.h>
#include <Scope/Control/Controller.h>
#include <Scope/Serialisation/JsonParser/JsonPacker.h>
#include <Scope/Serialisation/JsonParser/JsonUnpacker.h>
#include <Scope/Communication/Communicator.h>
#include <Scope/Communication/Interfaces/EthernetJson.h>
#include <Scope/Serialisation/Serializer.h>

/******************************************************************************
 Public functions
******************************************************************************/
void ScopeRunner_run(ScopeObject scope){

    scope.runCommunicationRx->run(scope.runCommunicationRx);
    scope.runUnpacker->run(scope.runUnpacker);
    scope.runCommandParser->run(scope.runCommandParser);
    scope.runScope->run(scope.runScope);
    scope.runDataAggregator->run(scope.runDataAggregator);
    scope.runPacker->run(scope.runPacker);
    scope.runCommunicationTx->run(scope.runCommunicationTx);
}
