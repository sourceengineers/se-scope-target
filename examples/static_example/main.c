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
  int timestamp = 0;

  ScopeHandle scope = Scope_create(500, 1, 0, UART, TIMESTAMP_AUTOMATIC, print);
  Scope_configureChannel(scope, 0, &test, UINT32);
  Scope_setChannelRunning(scope, 0);
  Scope_configureTrigger(scope, 48, TRIGGER_EDGE_POSITIVE, TRIGGER_NORMAL, 0);
  
  while (1) {
    
    for (size_t i = 0; i < rand() % 20; i++) {
      test = rand() % 50;
      Scope_poll(scope, 0);
    }    
    
    Scope_transmitData(scope);
    
    timestamp++;
    
    usleep(1000);
  }

  return 0;
}
