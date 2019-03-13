/*!*****************************************************************************
 * @file         JsonUartStack.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Build the Scope
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

} JsonUartPublicStack;

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __JsonUartStackPrivateData* JsonUartStackHandle;

/******************************************************************************
 Public functions
******************************************************************************/
/* Constructor: Creates a new instance of the channel */
void JsonUartStack_create(size_t sizeOfChannels, size_t  amountOfChannels, UartTransmitCallback callback,
                            uint32_t* timestamp, size_t  addressesInAddressAnnouncer);

void JsonUartStack_run();

/* Returns the JsonUartPublicStack which contains pointers to some elements which can be controller by the user code */
JsonUartPublicStack JsonUartStack_getObject();

void JsonUartStack_destroy();
#endif
