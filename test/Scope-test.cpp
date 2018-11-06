#include <gtest/gtest.h>
#include <gmock/gmock.h>

extern "C" {
    #include <Scope/Scope.h>
    #include <Scope/MsgpackParser/MsgpackCommon.h>
}

using namespace std;

const uint8_t* ev_poll = (const uint8_t*) "\x82\xa9\x74\x72\x61\x6e\x73\x70\x6f\x72\x74\xc0\xa7\x70\x61\x79\x6c\x6f\x61\x64\x81\xa6\x73\x63\x5f\x63\x6d\x64\x81\xa7\x65\x76\x5f\x70\x6f\x6c\x6c\x64";
const size_t ev_pollLength = 38;

const uint8_t* ev_trans = (const uint8_t*) "\x82\xa9\x74\x72\x61\x6e\x73\x70\x6f\x72\x74\xc0\xa7\x70\x61\x79\x6c\x6f\x61\x64\x81\xa6\x73\x63\x5f\x63\x6d\x64\x81\xa8\x65\x76\x5f\x74\x72\x61\x6e\x73\xa0";
const size_t ev_transLength = 39;

const uint8_t* faultyMessage = (const uint8_t*) "\x72\x61\x6e\x73\x70\x6f\x72\x74\xc0\xa7\x70\x61\x79\x6c\x6f\x61\x64\x81\xa6\x73\x63\x5f\x63\x6d\x64\x81\xa8\x65\x76\x5f\x74\x72\x61\x6e\x73\xa0";
const size_t faultyMessageLength = 36;

void copyByte(char* data, void* addr){
  const char* tmp =  (char*) addr;
  for(int i = 0; i<4; i++){
    data[i] = tmp[i];
  }
  data[4] = '\0';
}

void transmit(IByteStreamHandle stream){

  printf("\n");

  const size_t length = stream->length(stream);

  uint8_t data[length];

  stream->read(stream, data, length);

  //Msgpack_printAsBytes(data, length);
  Msgpack_printObjFromByte(stdout, data, length);
}

TEST(Scope, test_compile_time)
{

  ScopeHandle scope = Scope_create(200, 2, 0, UART,  TIMESTAMP_AUTOMATIC, transmit);

  const size_t lengthOfValues = 5;
  const float floatValues[lengthOfValues] = {0.0f,1.1f,2.2f,3.3f,4.4f};
  const gemmi_uint intValues[lengthOfValues] = {0,1,2,3,4};

  float floatAddr;
  gemmi_uint intAddr;

  Scope_configureChannel(scope, 0, &floatAddr, FLOAT);
  Scope_configureChannel(scope, 1, &intAddr, UINT32);

  Scope_setChannelRunning(scope, 0);
  Scope_setChannelRunning(scope, 1);

  Scope_configureTimestampIncrement(scope, 20);

  Scope_configureTrigger(scope, 2.1f, TRIGGER_EDGE_POSITIVE,TRIGGER_NORMAL,1);

  for (int i = 0; i < lengthOfValues; ++i) {
    floatAddr = floatValues[i];
    intAddr = intValues[i];

    Scope_poll(scope, 0);
  }

  Scope_transmitData(scope);
}

/*
{
	"transport": null,
	"payload": {
		"sc_cmd": {
			"ev_announce": ""
			}
	}
}
*/


const uint8_t* ev_announce = (const uint8_t*) "\x82\xa9\x74\x72\x61\x6e\x73\x70\x6f\x72\x74\xc0\xa7\x70\x61\x79\x6c\x6f\x61\x64\x81\xa6\x73\x63\x5f\x63\x6d\x64\x81\xab\x65\x76\x5f\x61\x6e\x6e\x6f\x75\x6e\x63\x65\xa0";
const size_t ev_announceLength = 42;

TEST(Scope, test_announce)
{

  ScopeHandle scope = Scope_create(500, 0, 2, ETHERNET,  TIMESTAMP_AUTOMATIC, transmit);

  const float testVarFloat = 4.4f;
  const gemmi_uint testVarInt = 5;

  Scope_setAnnounceAddresses(scope, "TestVarFloat", &testVarFloat, FLOAT, 0);
  Scope_setAnnounceAddresses(scope, "TestVarInt", &testVarInt, UINT32 , 1);

  Scope_announceWatchAddresses(scope);

  IByteStreamHandle stream = Scope_getInputStream(scope);

  stream->write(stream, ev_announce, ev_announceLength);

  Scope_command(scope);
}
