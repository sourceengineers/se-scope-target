//
// Created by schuepbs on 01.02.19.
//

#include <scope.h>
#include <usart.h>
#include <string.h>

AddressStorageHandle addressStorage;
ScopeBuilderHandle builder;
static ScopeObject scope_obj;
size_t inputBufferSize = 0;
size_t amountOfChannels;
size_t sizeOfChannels;
size_t outputBufferSize;
size_t addressesInAddressAnnouncer;
BufferedByteStreamHandle input;
BufferedByteStreamHandle output;
JsonUnpackerHandle unpacker;
JsonPackerHandle packer;
UartJsonHandle uartJson;

void scope_init(UartTransmitCallback callback, uint32_t* timestamp){

    /* Specify amount of channels, the size of channels and the maximum amount of addresses in the announcer */
    amountOfChannels = 5;
    sizeOfChannels = 100;
    addressesInAddressAnnouncer = 3;

    /* Let the Packer and Unpacker calculate how much buffer space they are going to use */
    outputBufferSize = JsonPacker_calculateBufferSize(amountOfChannels, sizeOfChannels,
                                                             addressesInAddressAnnouncer);
    inputBufferSize = JsonUnpacker_calculateBufferSize();

    /* Generate the input and output buffers, based on the previously calculated sizes */
    input = BufferedByteStream_create(inputBufferSize);
    output = BufferedByteStream_create(outputBufferSize);

    /* Generate the desired packer and unpacker */
    unpacker = JsonUnpacker_create(BufferedByteStream_getIByteStream(input));
    packer = JsonPacker_create(amountOfChannels, addressesInAddressAnnouncer,
                                                BufferedByteStream_getIByteStream(output));

    /* Generate the communication handler */
    uartJson = UartJson_create(callback, BufferedByteStream_getIByteStream(input),
                                                          BufferedByteStream_getIByteStream(output));

    /* Generate the address storage. The address storage is optional and doesn't have to be used */
    addressStorage = AddressStorage_create(addressesInAddressAnnouncer);

    /* Create the builder itself */
    builder = ScopeBuilder_create();

    /* Pass all the wanted elements into the builder */
    ScopeBuilder_setChannels(builder, amountOfChannels, sizeOfChannels);
    ScopeBuilder_setStreams(builder, BufferedByteStream_getIByteStream(input),
                            BufferedByteStream_getIByteStream(output));
    ScopeBuilder_setTimestampReference(builder, timestamp);
    ScopeBuilder_setCommunication(builder, UartJson_getCommunicator(uartJson));
    ScopeBuilder_setParser(builder, JsonPacker_getIPacker(packer), JsonUnpacker_getIUnpacker(unpacker));
    ScopeBuilder_setAddressStorage(builder, addressStorage);

    /* Build the scope */
    scope_obj = ScopeBuilder_build(builder);
}

void scope_run(){
    /* Run the scope */
    ScopeRunner_run(scope_obj);
}

void scope_destroy(){
    /* When the scope isn't used anymore, it can be destoryed */
    BufferedByteStream_destroy(input);
    BufferedByteStream_destroy(output);
    JsonUnpacker_destroy(unpacker);
    JsonPacker_destroy(packer);
    UartJson_destroy(uartJson);
    AddressStorage_destroy(addressStorage);
    ScopeBuilder_destroy(builder);
}
