#include <stdio.h>
#include <Gemmi.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <zconf.h>

/*
 * watch the s_out file with:
 *    $ watch -n 1 cat s_out
 *  to see continuous output of the example (The data is parsed to a json like format to
 *  make it more readable)
 *
 *  read from s_in
 */

void print(IByteStreamHandle stream){

    FILE* file = fopen("s_out", "w+");

    const size_t length = stream->length(stream);
    uint8_t data[length];

    stream->read(stream, data, length);

    fprintf(file, "\nMessage: %s", data);

    fprintf(file, "\n");

    fclose(file);
}

void readFile(IByteStreamHandle stream, const char* filename){

    FILE* file = fopen(filename, "rb");

    if(file == NULL){
        return;
    }

    char buffer[200];

    buffer[0] = '\0';

    if(fgets(buffer, 200, file) != NULL){
        stream->write(stream, (uint8_t*) buffer, strlen(buffer));
        stream->writeByte(stream, (uint8_t) '\0');
    }
    fclose(file);
    fopen(filename, "w");
    fclose(file);
}

int main(int argc, char* argv[]){

    uint32_t timestamp = 0;

    ScopeBuilderHandle builder = ScopeBuilder_create(print, &timestamp);
    ScopeObject obj = ScopeBuilder_build(builder);

    const char* filename = argc > 1 ? argv[1] : (const char*) "s_in";

    uint8_t var1;
    float var2;
    uint32_t var3;

    Scope_addAnnounceAddresses(obj.scope, (const char*) "VAR1", &var1, UINT8, 0);
    Scope_addAnnounceAddresses(obj.scope, (const char*) "VAR2", &var2, FLOAT, 1);
    Scope_addAnnounceAddresses(obj.scope, (const char*) "VAR3", &var3, UINT32, 2);

    Scope_announce(obj.scope);

    while(1){


        var3 = rand() % 100;
        var2 = (float) var3 * 1.5f;
        var1 = var3 / 10;

        readFile(obj.input, filename);

        ScopeRunner_run(obj);

        timestamp++;

        usleep(50);
    }

    return 0;
}
