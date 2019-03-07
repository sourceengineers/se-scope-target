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

#include <Scope/Control/Controller.h>
#include <Scope/Core/Scope.h>
#include <Scope/Serialisation/Serializer.h>
#include <Scope/Communication/ICommunicator.h>
#include <Scope/Control/IPacker.h>
#include <Scope/Control/IUnpacker.h>
#include <Scope/Core/AddressStorage.h>
#include <Scope/GeneralPurpose/IByteStream.h>
#include <Scope/GeneralPurpose/IRunnable.h>

#include <stddef.h>
#include <stdint.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __ScopeBuilderPrivateData* ScopeBuilderHandle;

/******************************************************************************
 Define interface
******************************************************************************/
typedef struct ScopeObjectStruct{

    ScopeHandle scope;

    IRunnableHandle runCommunicationRx;
    IRunnableHandle runCommunicationTx;
    IRunnableHandle runUnpacker;
    IRunnableHandle runPacker;
    IRunnableHandle runCommandParser;
    IRunnableHandle runDataAggregator;
    IRunnableHandle runScope;

} ScopeObject;

/******************************************************************************
 Public functions
******************************************************************************/
/* Constructor: Creates a new instance of the channel */
ScopeBuilderHandle ScopeBuilder_create(void);

/* Builds the scope with gives objects */
ScopeObject ScopeBuilder_build(ScopeBuilderHandle self);

/* Set the number and the size of the channels */
void ScopeBuilder_setChannels(ScopeBuilderHandle self, size_t amountOfChannels, size_t sizeOfChannels);

/* Set the used input and output streams */
void ScopeBuilder_setStreams(ScopeBuilderHandle self, IByteStreamHandle input, IByteStreamHandle output);

/* Set a reference to the used timestamp */
void ScopeBuilder_setTimestampReference(ScopeBuilderHandle self, uint32_t* timestamp);

/* Set the used parsers */
void ScopeBuilder_setParser(ScopeBuilderHandle self, IPackerHandle packer, IUnpackerHandle unpacker);

/* Set the used communication protocol */
void ScopeBuilder_setCommunication(ScopeBuilderHandle self, ICommunicatorHandle interface);

/* If wanted, set the address announcer */
void ScopeBuilder_setAddressStorage(ScopeBuilderHandle self, AddressStorageHandle addressStorage);

/* Deconstructor: Deletes the instance of the channel */
void ScopeBuilder_destroy(ScopeBuilderHandle self);


#endif
