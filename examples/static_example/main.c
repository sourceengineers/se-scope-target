#include <stdio.h>
#include <Gemmi.h>
#include <unistd.h>
#include <stdlib.h>
#include <Scope/MsgpackParser/MsgpackCommon.h>

void print(IByteStreamHandle stream){
  
  const size_t length = stream->length(stream);
  uint8_t data[length];
  stream->read(stream, data, length);
  printf("\r");
  Msgpack_printObjFromByte(data,length);    
}

int main(){

  int test = 0;
  int timestamp = 0;

  ScopeHandle scope = Scope_create(500, 1, 0, ETHERNET, TIMESTAMP_AUTOMATIC, print);
  Scope_configureChannel(scope, 0, &test, UINT32);
  Scope_setChannelRunning(scope, 0);
  
  while (1) {
    
    for (size_t i = 0; i < 550; i++) {
      test = rand();
      Scope_poll(scope, 0);
    }    
    
    Scope_transmitData(scope);
    
    timestamp++;
    
    usleep(50000);
  }

  return 0;
}
