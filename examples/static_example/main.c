#include <stdio.h>
#include <Gemmi.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <zconf.h>

/*
 * watch the s_out file with:
 *    $ watch -n 1 cat s_out
 *  to see continuous output of the example (The data is parsed to a json like format to
 *  make it more readable)
 */

void print(IByteStreamHandle stream){

    FILE* file = fopen("s_out", "w+");

    const size_t length = stream->length(stream);
    uint8_t data[length];

    stream->read(stream, data, length);
    fprintf(file, "%s", data);

    fclose(file);
}

int main(){

    int test = 0;
    uint32_t timestamp = 0;

    ScopeBuilderHandle builder = ScopeBuilder_create(print, &timestamp);
    ScopeObject obj = ScopeBuilder_build(builder);

    Scope_configureChannel(obj.scope, 0, &test, UINT32);
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

    return 0;
}
