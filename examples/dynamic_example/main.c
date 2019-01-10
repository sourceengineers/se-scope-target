#include <stdio.h>
#include <Gemmi.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

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

  uint8_t byte = 0;

  while(fread(&byte, 1, 1, file) != 0){
    stream->writeByte(stream, byte);
  }
  fclose(file);
  fopen(filename, "w");
  fclose(file);
}

int main(int argc, char *argv[] ){

  ScopeHandle scope = Scope_create(500, 3, 3, ETHERNET, TIMESTAMP_AUTOMATIC, print);
  IByteStreamHandle stream = Scope_getInputStream(scope);


  const char* filename = argc > 1 ? argv[1] : (const char*) "s_in";

  uint8_t var1;
  float var2;
  uint32_t var3;

  // addAnnoucedAddress würde mir besser gefallen
  Scope_setAnnounceAddresses(scope,(const char*) "VAR1", &var1, UINT8, 0);
  Scope_setAnnounceAddresses(scope,(const char*) "VAR2", &var2, FLOAT, 1);
  Scope_setAnnounceAddresses(scope,(const char*) "VAR3", &var3, UINT32, 2);

  Scope_announceAddresses(scope);

  while (1) {


    var3 = rand() % 100;
    var2 = (float) var3 * 1.5f;
    var1 = var3 / 10;

    readFile(stream, filename);

    // ich dieses if ist überflüssig, du checkst das ja am anfang von command nochmal
    if(stream->length(stream) > 0){
      // hier läuft ja die ganze rx kommunikation. wäre Scope_receiveData nicht besser?
      Scope_command(scope);
    }

    Scope_poll(scope, 0);
    Scope_transmitData(scope);
    usleep(10000);
  }

  return 0;
}
