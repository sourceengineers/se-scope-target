/*!*****************************************************************************
 * @file         JsonUartStack.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Builds a Uart Json stack.
 *
 ******************************************************************************/

#ifndef JsonUartStack_H
#define JsonUartStack_H

#include <Scope/Core/Scope.h>
#include <Scope/Builders/ScopeBuilder.h>
#include <Scope/GeneralPurpose/BufferedByteStream.h>
#include <Scope/Control/AddressStorage.h>
#include <Scope/Communication/Interfaces/UartJson.h>
#include <Scope/Serialisation/JsonParser/JsonPacker.h>
#include <Scope/Serialisation/JsonParser/JsonUnpacker.h>

/******************************************************************************
 Define interface
******************************************************************************/
typedef struct JsonUartPublicStackStruct {

    AddressStorageHandle addressStorage;
    ScopeHandle scope;
    BufferedByteStreamHandle input;
    BufferedByteStreamHandle output;
    UartJsonHandle uartJson;
    size_t outputBufferSize;
    size_t inputBufferSize;

} JsonUartPublicStack;

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __JsonUartStackPrivateData* JsonUartStackHandle;

/******************************************************************************
 Public functions
******************************************************************************/
/**
 * Constructor. Creates a static instance of the stack
 * @param sizeOfChannels Size of the channels
 * @param amountOfChannels
 * @param callback Callback to the UsartTransmit method defined by the user.
 * @param timestamp Timestamp as base for the scope. This has to be incremented by the user.
 * @param addressesInAddressAnnouncer Amount of addresses that can be stored to be transmitted
 */
void JsonUartStack_create(size_t sizeOfChannels, size_t  amountOfChannels, UartTransmitCallback callback,
                          uint32_t* timestamp, size_t  addressesInAddressAnnouncer);

/**
 * Runs the JsonUart stack
 */
void JsonUartStack_run();

/**
 * Returns a JsonUartPublicStack instance to be able to control some parts of the code. Add addresses to the
 * AddressStorage as example.
 * @return AddressStorageHandle
 */
JsonUartPublicStack JsonUartStack_getObject();

/**
 * Deonstructor
 */
void JsonUartStack_destroy();
#endif
