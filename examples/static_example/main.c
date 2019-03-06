#include <stdio.h>
#include <Gemmi.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <Scope/Communication/Interfaces/EthernetJson.h>
#include <Scope/Serialisation/JsonParser/JsonPacker.h>
#include <unistd.h>

/*
 * watch the s_out file with:
 *    $ watch -n 1 cat s_out
 *  to see continuous output of the example (The data is parsed to a json like format to
 *  make it more readable)
 */

void print(EthernetJsonHandle ethernetJson){

    FILE* file = fopen("s_out", "w+");

    const size_t length = EthernetJson_amountOfTxDataPending(ethernetJson);
    uint8_t data[length];

    EthernetJson_getTxData(ethernetJson, data, length);

    fprintf(file, "%s", data);
    fclose(file);
}

int main(){


/***********************************************************************************************************************
* Build Scope
***********************************************************************************************************************/
    size_t amountOfChannels = 2;
    size_t sizeOfChannels = 100;
    size_t outputBufferSize = JsonPacker_calculateBufferSize(amountOfChannels, sizeOfChannels, 0);

    BufferedByteStreamHandle output = BufferedByteStream_create(outputBufferSize);
    JsonPackerHandle packer = JsonPacker_create(3, 0, BufferedByteStream_getIByteStream(output));
    EthernetJsonHandle ethernetJson = EthernetJson_create(print, NULL,
                                                          BufferedByteStream_getIByteStream(output));
    uint32_t timestamp = 0;

    ScopeBuilderHandle builder = ScopeBuilder_create();
    ScopeBuilder_setChannels(builder, amountOfChannels, sizeOfChannels);
    ScopeBuilder_setStreams(builder, NULL, BufferedByteStream_getIByteStream(output));
    ScopeBuilder_setTimestampReference(builder, &timestamp);
    ScopeBuilder_setCommunication(builder, EthernetJson_getCommunicator(ethernetJson));
    ScopeBuilder_setParser(builder, JsonPacker_getIPacker(packer), NULL);

    ScopeObject obj = ScopeBuilder_build(builder);


/***********************************************************************************************************************
* User code
***********************************************************************************************************************/
    int test = 0;
    Scope_configureChannel(obj.scope, 0, &test, SE_UINT32);
    Scope_setChannelRunning(obj.scope, 0);
    Scope_configureTrigger(obj.scope, 48, TRIGGER_EDGE_POSITIVE, TRIGGER_NORMAL, 0);

    while(1){

        for(size_t i = 0; i < rand() % 20; i++){
            test = rand() % 50;
            ScopeRunner_run(obj);
        }

        timestamp++;

        usleep(1000);
    }

    ScopeBuilder_destroy(builder);
    JsonPacker_destroy(packer);
    EthernetJson_destroy(ethernetJson);
    BufferedByteStream_destroy(output);

    return 0;
}
