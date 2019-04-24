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
#include <Scope/Control/AddressStorage.h>
#include <Scope/GeneralPurpose/IByteStream.h>
#include <Scope/GeneralPurpose/IRunnable.h>
#include <Scope/GeneralPurpose/IMutex.h>

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

    IScopeHandle scope;
    ControllerHandle controller;

    IRunnableHandle runCommunicationRx;
    IRunnableHandle runCommunicationTx;
    IRunnableHandle runUnpacker;
    IRunnableHandle runPacker;
    IRunnableHandle runCommandParser;
    IRunnableHandle runDataAggregator;
    IRunnableHandle runScope;

    IMutexHandle dataMutex;
    IMutexHandle configMutex;

} ScopeObject;

/******************************************************************************
 Public functions
******************************************************************************/
/**
 * Constructor
 * @return ScopeBuilderHandle
 */
ScopeBuilderHandle ScopeBuilder_create(void);

/**
 * Builds the scope and returns an object with runnables
 * @param self
 * @return ScopeObject
 */
ScopeObject ScopeBuilder_build(ScopeBuilderHandle self);

/**
 * Sets the amount and size of channels that have to be build
 * @param self
 * @param amountOfChannels
 * @param sizeOfChannels
 */
void ScopeBuilder_setChannels(ScopeBuilderHandle self, size_t amountOfChannels, size_t sizeOfChannels);

/**
 * Set the used input and output streams
 * @param self
 * @param input Stream which contains the received data
 * @param output Stream which contains the data to be sent
 */
void ScopeBuilder_setStreams(ScopeBuilderHandle self, IByteStreamHandle input, IByteStreamHandle output);

/**
 * Sets the timestamp reference which will be passed on to the scope
 * @param self
 * @param timestamp
 */
/* */
void ScopeBuilder_setTimestampReference(ScopeBuilderHandle self, uint32_t* timestamp);

/**
 * Set the used protocol
 * @param self
 * @param packer Packer which will serialize the data of the scope.
 * @param unpacker Unpacker used to deserialize the commands.
 */
void ScopeBuilder_setParser(ScopeBuilderHandle self, IPackerHandle packer, IUnpackerHandle unpacker);

/**
 * Set the used communication interface
 * @param self
 * @param interface
 */
void ScopeBuilder_setCommunication(ScopeBuilderHandle self, ICommunicatorHandle interface);

/**
 * Add the AddressStorage. With the help of the AddressStorage, addresses and their aliases can be sent to the host
 * This can be handy when the addresses aren't known at compile time.
 * This is optional.
 * @param self
 * @param addressStorage
 */
void ScopeBuilder_setAddressStorage(ScopeBuilderHandle self, AddressStorageHandle addressStorage);

/**
 * Appends the mutex which protects the Tx path of the runner. If no mutex is passed,
 * only the ScopeRunner can be used. Otherwise the ScopeThreadRunner can be used.
 * @param self
 * @param mutex
 */
void ScopeBuilder_setDataMutex(ScopeBuilderHandle self, IMutexHandle mutex);

/**
 * Appends the mutex which protects the Rx path of the runner. If no mutex is passed,
 * only the ScopeRunner can be used. Otherwise the ScopeThreadRunner can be used.
 * @param self
 * @param mutex
 */
void ScopeBuilder_setConfigMutex(ScopeBuilderHandle self, IMutexHandle mutex);

/**
 * Deconstructor
 * @param self
 */
void ScopeBuilder_destroy(ScopeBuilderHandle self);


#endif