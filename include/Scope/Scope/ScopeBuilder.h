/*!*****************************************************************************
 * @file         ScopeBuilder.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Build the Scope
 *
 ******************************************************************************/

#ifndef SCOPEBUILDER_H
#define SCOPEBUILDER_H

#include <Scope/Core/Scope.h>
#include <Scope/Communication/CommunicationTypes.h>
#include <Scope/Control/Controller.h>
#include <Scope/Communication/Communicator.h>
#include <Scope/Serialisation/Serializer.h>
#include <Scope/Core/Scope.h>
/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __ScopeBuilderPrivateData* ScopeBuilderHandle;

/******************************************************************************
 Define interface
******************************************************************************/
typedef struct ScopeObjectStruct {

    ScopeHandle scope;

    IByteStreamHandle input;

    IRunnableHandle runCommunicationRx;
    IRunnableHandle runCommunicationTx;
    IRunnableHandle runSerializerRx;
    IRunnableHandle runSerializerTx;
    IRunnableHandle runControlRx;
    IRunnableHandle runControlTx;
    IRunnableHandle runScope;

} ScopeObject ;

/******************************************************************************
 Public functions
******************************************************************************/
/* Constructor: Creates a new instance of the channel */
ScopeBuilderHandle ScopeBuilder_create(TransmitCallback callback, uint32_t* timestampReference);

/* Builds the scope with gives objects */
ScopeObject ScopeBuilder_build(ScopeBuilderHandle self);

/* Deconstructor: Deletes the instance of the channel */
void ScopeBuilder_destroy(ScopeBuilderHandle self);



#endif
