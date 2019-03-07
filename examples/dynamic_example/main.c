#include <Scope/Communication/Interfaces/EthernetJson.h>
#include <Scope/Serialisation/JsonParser/JsonPacker.h>
#include <Scope/Serialisation/JsonParser/JsonUnpacker.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "Scope/Control/AddressStorage.h"
#include "Scope/GeneralPurpose/BufferedByteStream.h"
#include "Scope/GeneralPurpose/DataTypes.h"
#include "Scope/ScopeBuilder.h"
#include "Scope/ScopeRunner.h"

/*
 * watch the s_out file with:
 *    $ watch -n 1 cat s_out
 *  to see continuous output of the example (The data is parsed to a json like format to
 *  make it more readable)
 *
 *  read from s_in
 */

void print(EthernetJsonHandle self){

    FILE* file = fopen("s_out", "w+");

    const size_t length = EthernetJson_amountOfTxDataPending(self);
    uint8_t data[length];

    EthernetJson_getTxData(self, data, length);

    fprintf(file, "%s\n", data);

    fclose(file);
}


/* The data hast to be written into the input buffer for the scope to be interpreted */
void readFile(EthernetJsonHandle self, const char* filename){

    FILE* file = fopen(filename, "rb");

    if(file == NULL){
        return;
    }

    char buffer[200];

    buffer[0] = '\0';

    if(fgets(buffer, 200, file) != NULL){
        EthernetJson_putRxData(self, (uint8_t*) buffer, strlen(buffer));
    }
    fclose(file);
    fopen(filename, "w");
    fclose(file);
}

int main(int argc, char* argv[]){

/***********************************************************************************************************************
* Build Scope
***********************************************************************************************************************/
    size_t amountOfChannels = 4;
    size_t sizeOfChannels = 100;
    size_t addressesInAddressAnnouncer = 3;
    size_t outputBufferSize = JsonPacker_calculateBufferSize(amountOfChannels, sizeOfChannels,
                                                             addressesInAddressAnnouncer);
    size_t inputBufferSize = JsonUnpacker_calculateBufferSize();

    BufferedByteStreamHandle input = BufferedByteStream_create(inputBufferSize);
    BufferedByteStreamHandle output = BufferedByteStream_create(outputBufferSize);
    JsonUnpackerHandle unpacker = JsonUnpacker_create(BufferedByteStream_getIByteStream(input));
    JsonPackerHandle packer = JsonPacker_create(amountOfChannels, addressesInAddressAnnouncer,
                                                BufferedByteStream_getIByteStream(output));
    EthernetJsonHandle ethernetJson = EthernetJson_create(print, BufferedByteStream_getIByteStream(input),
                                                          BufferedByteStream_getIByteStream(output));
    uint32_t timestamp = 0;

    AddressStorageHandle addressStorage = AddressStorage_create(addressesInAddressAnnouncer);

    ScopeBuilderHandle builder = ScopeBuilder_create();
    ScopeBuilder_setChannels(builder, amountOfChannels, sizeOfChannels);
    ScopeBuilder_setStreams(builder, BufferedByteStream_getIByteStream(input),
                            BufferedByteStream_getIByteStream(output));
    ScopeBuilder_setTimestampReference(builder, &timestamp);
    ScopeBuilder_setCommunication(builder, EthernetJson_getCommunicator(ethernetJson));
    ScopeBuilder_setParser(builder, JsonPacker_getIPacker(packer), JsonUnpacker_getIUnpacker(unpacker));
    ScopeBuilder_setAddressStorage(builder, addressStorage);

    ScopeObject obj = ScopeBuilder_build(builder);


/***********************************************************************************************************************
* User code
***********************************************************************************************************************/

    const char* filename = argc > 1 ? argv[1] : (const char*) "s_in";

    uint8_t var1;
    float var2;
    uint32_t var3;

    AddressStorage_addAnnounceAddress(addressStorage, (const char*) "VAR1", &var1, SE_UINT8, 0);
    AddressStorage_addAnnounceAddress(addressStorage, (const char*) "VAR2", &var2, SE_FLOAT, 1);
    AddressStorage_addAnnounceAddress(addressStorage, (const char*) "VAR3", &var3, SE_UINT32, 2);
    AddressStorage_announce(addressStorage);

    while(1){


        var3 = rand() % 100;
        var2 = (float) var3 * 1.5f;
        var1 = var3 / 10;

        readFile(ethernetJson, filename);

        ScopeRunner_run(obj);

        timestamp++;

        usleep(50);
    }

/***********************************************************************************************************************
* Destroy objects
***********************************************************************************************************************/

    ScopeBuilder_destroy(builder);

    JsonUnpacker_destroy(unpacker);
    JsonPacker_destroy(packer);
    EthernetJson_destroy(ethernetJson);
    BufferedByteStream_destroy(input);
    BufferedByteStream_destroy(output);
    AddressStorage_destroy(addressStorage);

    return 0;
}
