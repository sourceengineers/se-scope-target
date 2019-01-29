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
    scope.runSerializerRx->run(scope.runSerializerRx);
    scope.runControlRx->run(scope.runControlRx);
    scope.runScope->run(scope.runScope);
    scope.runControlTx->run(scope.runControlTx);
    scope.runSerializerTx->run(scope.runSerializerTx);
    scope.runCommunicationTx->run(scope.runCommunicationTx);

}
