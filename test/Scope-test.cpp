#include <gtest/gtest.h>
#include <gmock/gmock.h>

extern "C" {
    #include <Scope/Scope.h>
    #include <Scope/MsgpackParser/MsgpackCommon.h>
}

using namespace std;

/* Msgpart 1 */
/*
{
	"transport": null,
	"payload": {
		"sc_cmd": {
			"cf_addr": {
				"0":
 */

const char partOne[] = "\x82\xa9\x74\x72\x61\x6e\x73\x70\x6f\x72\x74\xc0\xa7\x70\x61\x79\x6c\x6f\x61\x64\x81\xa6\x73\x63\x5f\x63\x6d\x64\x84\xa7\x63\x66\x5f\x61\x64\x64\x72\x82\xa1\x30\x92";

/* Middle of message */
/*
        ,
        "0_type" : "FLOAT",
				"1":
*/
const char partTwo[] =  "\xa5\x46\x4c\x4f\x41\x54\xa1\x31\x92";


/* End of message */
/*
				"1_type": "UINT32_T"
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
const char partThree[] = "\xa6\x55\x49\x4e\x54\x33\x32\xa6\x63\x66\x5f\x74\x67\x72\x84\xa5\x63\x6c\x5f\x69\x64\x01\xa4\x6d\x6f\x64\x65\xa6\x4e\x6f\x72\x6d\x61\x6c\xa5\x6c\x65\x76\x65\x6c\xcb\x40\x04\x00\x00\x00\x00\x00\x00\xa4\x65\x64\x67\x65\xc3\xaa\x63\x66\x5f\x72\x75\x6e\x6e\x69\x6e\x67\x82\xa1\x30\xc3\xa1\x31\xc3\xa8\x63\x66\x5f\x74\x5f\x69\x6e\x63\x0a";

const char intPrefix[] = "\xce";


/* Msgpart 1 */
/*
{
	"transport": null,
	"payload": {
		"sc_cmd": {
			"ev_poll": 100
		}
	}
}

{
	"transport": null,
	"payload": {
		"sc_cmd": {
			"ev_trans": ""
		}
	}
}
*/

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

TEST(Scope, test_msgpack)
{

  const size_t maxLength = 230;

  const float testDataFloat[7] = {1.1f,2.2f,3.3f,4.4f,5.5f,6.6f,7.7f};
  const uint32_t testDataInt[7] = {10,11,12,13,14,15,16};

  volatile float testVarFloat;
  volatile uint32_t testVarInt;

  uint8_t data[maxLength];

  uint32_t  addrOne = __builtin_bswap32((uint32_t) ((uint32_t*) &testVarFloat));
  uint32_t  addrTwo = __builtin_bswap32((uint32_t) ((uint32_t*)&testVarInt));

  char addrBytesOne[5];
  copyByte(addrBytesOne, &addrOne);

  char addrBytesTwo[5];
  copyByte(addrBytesTwo, &addrTwo);

  int len = 0;

  memcpy( data, partOne, 41);
  len = 41;
  memcpy( data + len, intPrefix, 1);
  len += 1;
  memcpy(data + len, addrBytesOne, 4);
  len += 4;
  memcpy(data + len, partTwo, 9);
  len += 9;
  memcpy( data + len, intPrefix, 1);
  len += 1;
  memcpy( data + len, addrBytesTwo, 4);
  len += 4;
  memcpy(data + len, partThree, 83);
  len += 83;

  data[len] = '\0';

  Msgpack_printObjFromByte(stdout, data, len);

  ScopeHandle scope = Scope_create(100, 2, 0, ETHERNET, TIMESTAMP_MANUAL, transmit);

  IByteStreamHandle inputStream = Scope_getInputStream(scope);

  inputStream->write(inputStream, data, len);

  Scope_command(scope);

  for (uint32_t j = 0; j < 5; ++j) {
    testVarFloat = testDataFloat[j];
    testVarInt = testDataInt[j];

    Scope_poll(scope, j);
  }

  Scope_transmitData(scope);

  // Test ev_poll twice
  testVarFloat = testDataFloat[5];
  testVarInt = testDataInt[5];
  inputStream->write(inputStream, ev_poll, ev_pollLength);
  Scope_command(scope);

  testVarFloat = testDataFloat[6];
  testVarInt = testDataInt[6];
  inputStream->write(inputStream, ev_poll, ev_pollLength);
  Scope_command(scope);

  // Test ev_trans
  inputStream->write(inputStream, ev_trans, ev_transLength);
  Scope_command(scope);

  // Test faulty message
  inputStream->write(inputStream, faultyMessage, faultyMessageLength);
  Scope_command(scope);

}

TEST(Scope, test_compile_time)
{

  ScopeHandle scope = Scope_create(200, 2, 0, UART,  TIMESTAMP_AUTOMATIC, transmit);

  const size_t lengthOfValues = 5;
  const float floatValues[lengthOfValues] = {0.0f,1.1f,2.2f,3.3f,4.4f};
  const uint32_t intValues[lengthOfValues] = {0,1,2,3,4};

  float floatAddr;
  uint32_t intAddr;

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
  const uint32_t testVarInt = 5;

  Scope_setWatchAddresses(scope, "TestVarFloat", &testVarFloat, FLOAT, 0);
  Scope_setWatchAddresses(scope, "TestVarInt", &testVarInt, UINT32 , 1);

  Scope_announceWatchAddresses(scope);

  IByteStreamHandle stream = Scope_getInputStream(scope);

  stream->write(stream, ev_announce, ev_announceLength);

  Scope_command(scope);
}
