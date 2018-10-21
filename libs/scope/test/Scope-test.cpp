#include <gtest/gtest.h>
#include <gmock/gmock.h>

extern "C" {
    #include "Scope.h"
    #include <MsgpackParser/MsgpackCommon.h>
}

using namespace std;

/* Msgpart 1 */
/*
{
	"transport": "...",
	"payload": {
		"sc_cmd": {
			"cf_addr": {
				"1":
 */

const char partOne[] = "\x82\xa9\x74\x72\x61\x6e\x73\x70\x6f\x72\x74\xa3\x2e\x2e\x2e\xa7\x70\x61\x79\x6c\x6f\x61\x64\x81\xa6\x73\x63\x5f\x63\x6d\x64\x84\xa7\x63\x66\x5f\x61\x64\x64\x72\x84\xa1\x31";

/* Middle of message */
/*
        ,
        "1_type" : "FLOAT",
				"2":
*/
const char partTwo[] =  "\xa1\x32";


/* End of message */
/*
				"2_type": "UINT32_T"
			},
			"cf_tgr": {
				"cl_id": 1,
				"mode": "Normal",
				"level": 2.5,
				"edge": true
			},
			"cf_running": {
				"1": true,
				"2": true
			},
			"cf_t_inc": 10
		}
	}
}
*/
const char partThree[] = "\xa6\x31\x5f\x74\x79\x70\x65\xa5\x46\x4c\x4f\x41\x54\xa6\x32\x5f\x74\x79\x70\x65\xa6\x55\x49\x4e\x54\x33\x32\xa6\x63\x66\x5f\x74\x67\x72\x84\xa5\x63\x6c\x5f\x69\x64\x01\xa4\x6d\x6f\x64\x65\xa6\x4e\x6f\x72\x6d\x61\x6c\xa5\x6c\x65\x76\x65\x6c\xcb\x40\x04\x00\x00\x00\x00\x00\x00\xa4\x65\x64\x67\x65\xc3\xaa\x63\x66\x5f\x72\x75\x6e\x6e\x69\x6e\x67\x82\xa1\x31\xc3\xa1\x32\xc3\xa8\x63\x66\x5f\x74\x5f\x69\x6e\x63\x0a";

const char intPrefix[] = "\xce";

void copyByte(char* data, void* addr){
  const char* tmp =  (char*) addr;
  for(int i = 0; i<4; i++){
    data[i] = tmp[i];
  }
  data[4] = '\0';
}

void transmit(IByteStreamHandle stream){

  const size_t length = stream->length(stream);

  uint8_t data[length];

  stream->read(stream, data, length);

  Msgpack_printAsBytes(data, length);
  Msgpack_printObjFromByte(data, length);
}

TEST(Scope, test_all)
{

  const size_t maxLength = 230;

  const float testDataFloat[5] = {1.1f,2.2f,3.3f,4.4f,5.5f};
  const uint32_t testDataInt[5] = {10,11,12,13,14};

  volatile float testVarFloat;
  volatile uint32_t testVarInt;

  uint8_t data[maxLength];

  uint32_t  addrOne = __builtin_bswap32((uint32_t) &testVarFloat);
  uint32_t  addrTwo = __builtin_bswap32((uint32_t) &testVarInt);

  char addrBytesOne[5];
  copyByte(addrBytesOne, &addrOne);

  char addrBytesTwo[5];
  copyByte(addrBytesTwo, &addrTwo);

  int len = 0;

  memcpy( data, partOne, 43);
  len = 43;
  memcpy( data + len, intPrefix, 1);
  len += 1;
  memcpy(data + len, addrBytesOne, 4);
  len += 4;
  memcpy(data + len, partTwo, 2);
  len += 2;
  memcpy( data + len, intPrefix, 1);
  len += 1;
  memcpy( data + len, addrBytesTwo, 4);
  len += 4;
  memcpy(data + len, partThree, 103);
  len += 103;

  data[len] = '\0';

  ScopeHandle scope = Scope_create(100,2, UART, TIMESTAMP_MANUAL, transmit);

  IByteStreamHandle inputStream = Scope_getInputStream(scope);

  inputStream->write(inputStream, data, len);

  Scope_command(scope);

  for (uint32_t j = 0; j < 5; ++j) {
    testVarFloat = testDataFloat[j];
    testVarInt = testDataInt[j];

    Scope_poll(scope, j);
  }

  Scope_packMessage(scope);

}
