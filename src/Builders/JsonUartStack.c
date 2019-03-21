/*!****************************************************************************************************************************************
 * @file         ScopeBuilder.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Builders/JsonUartStack.h>
#include <Scope/Serialisation/JsonParser/JsonPacker.h>
#include <Scope/Serialisation/JsonParser/JsonUnpacker.h>
#include <Scope/Communication/Interfaces/UartJson.h>
#include <Scope/Builders/ScopeRunner.h>


/* The JsonuartStack module stores the JsonUartStackHandle as static in the module.
 * This is supposed to make it easier for the user to work with the stack.
 * The user therefore doesn't have to take care of the pointer to the stack by themselves.
 * Due to this, there can only be one instance of the stack. */
static JsonUartStackHandle self;

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __JsonUartStackPrivateData{

    AddressStorageHandle addressStorage;
    ScopeBuilderHandle builder;
    BufferedByteStreamHandle input;
    BufferedByteStreamHandle output;
    JsonUnpackerHandle unpacker;
    JsonPackerHandle packer;
    UartJsonHandle uartJson;
    ScopeObject scope_obj;
    size_t outputBufferSize;
    size_t inputBufferSize;

} JsonUartStackPrivateData;

/******************************************************************************
 Private functions
******************************************************************************/
void JsonUartStack_create(size_t sizeOfChannels, size_t  amountOfChannels, UartTransmitCallback callback, uint32_t* timestamp,
                          size_t  addressesInAddressAnnouncer){

    self = malloc(sizeof(JsonUartStackPrivateData));

    /* Let the Packer and Unpacker calculate how much buffer space they are going to use */
    self->outputBufferSize = JsonPacker_calculateBufferSize(amountOfChannels, sizeOfChannels,
                                                            addressesInAddressAnnouncer);
    self->inputBufferSize = JsonUnpacker_calculateBufferSize();
    size_t usedSpace = self->outputBufferSize + self->inputBufferSize + sizeOfChannels * amountOfChannels;


    /* Generate the input and output buffers, based on the previously calculated sizes */
    self->input = BufferedByteStream_create(self->inputBufferSize);
    self->output = BufferedByteStream_create(self->outputBufferSize);

    /* Generate the desired packer and unpacker */
    self->packer = JsonPacker_create(amountOfChannels, addressesInAddressAnnouncer,
                                     BufferedByteStream_getIByteStream(self->output));
    self->unpacker = JsonUnpacker_create(BufferedByteStream_getIByteStream(self->input));

    /* Generate the communication handler */
    self->uartJson = UartJson_create(callback, BufferedByteStream_getIByteStream(self->input),
                                     BufferedByteStream_getIByteStream(self->output));

    /* Generate the address storage. The address storage is optional and doesn't have to be used */
    self->addressStorage = AddressStorage_create(addressesInAddressAnnouncer);

    /* Create the builder itself */
    self->builder = ScopeBuilder_create();

    /* Pass all the wanted elements into the builder */
    ScopeBuilder_setChannels(self->builder, amountOfChannels, sizeOfChannels);
    ScopeBuilder_setStreams(self->builder, BufferedByteStream_getIByteStream(self->input),
                            BufferedByteStream_getIByteStream(self->output));
    ScopeBuilder_setTimestampReference(self->builder, timestamp);
    ScopeBuilder_setCommunication(self->builder, UartJson_getCommunicator(self->uartJson));
    ScopeBuilder_setParser(self->builder, JsonPacker_getIPacker(self->packer), JsonUnpacker_getIUnpacker(self->unpacker));
    ScopeBuilder_setAddressStorage(self->builder, self->addressStorage);

    /* Build the scope */
    self->scope_obj = ScopeBuilder_build(self->builder);

}

void JsonUartStack_run(){
    ScopeRunner_run(self->scope_obj);
}

JsonUartPublicStack JsonUartStack_getObject(){

    JsonUartPublicStack stack = {.addressStorage = self->addressStorage,
            .scope = self->scope_obj.scope,
            .input = self->input,
            .output = self->output,
            .uartJson = self->uartJson,
            .outputBufferSize = self->outputBufferSize,
            .inputBufferSize = self->inputBufferSize};

    return stack;
}

void JsonUartStack_destroy(){
    BufferedByteStream_destroy(self->input);
    BufferedByteStream_destroy(self->output);
    JsonUnpacker_destroy(self->unpacker);
    JsonPacker_destroy(self->packer);
    UartJson_destroy(self->uartJson);
    AddressStorage_destroy(self->addressStorage);
    ScopeBuilder_destroy(self->builder);
}
