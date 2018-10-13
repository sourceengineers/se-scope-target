#include <gtest/gtest.h>
#include <gmock/gmock.h>

extern "C" {
    #include "Scope.h"
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

const char partOne[] = "\x82\xa9\x74\x72\x61\x6e\x73\x70\x6f\x72\x74\xa3\x2e\x2e\x2e\xa7\x70\x61\x79\x6c\x6f\x61\x64\x81\xa6\x73\x63\x5f\x63\x6d\x64\x83\xa7\x63\x66\x5f\x61\x64\x64\x72\x82\xa1\x31";

/* Middle of message */
/*
 , "2":
*/
const char partTwo[] =  "\xa1\x32";


/* End of message */
/*
			},
			"cf_tgr": {
				"cl_id": 1,
				"mode": "Continous",
				"level": 2.5,
				"edge": true
			}
		}
	}
}
*/
const char partThree[] = "\xaa\x63\x66\x5f\x72\x75\x6e\x6e\x69\x6e\x67\x82\xa1\x31\xc3\xa1\x32\xc3\xa6\x63\x66\x5f\x74\x67\x72\x84\xa5\x63\x6c\x5f\x69\x64\x01\xa4\x6d\x6f\x64\x65\xa9\x43\x6f\x6e\x74\x69\x6e\x6f\x75\x73\xa5\x6c\x65\x76\x65\x6c\xcb\x40\x04\x00\x00\x00\x00\x00\x00\xa4\x65\x64\x67\x65\xc3";

const char intPrefix[] = "\xce";

void copyByte(char* data, void* addr){
  const char* tmp =  (char*) addr;
  for(int i = 0; i<4; i++){
    data[i] = tmp[i];
  }
  data[4] = '\0';
}

TEST(Scope, test_all)
{

  const size_t maxLength = 230;

  volatile float vars[2];

  vars[0] = 1.45f;
  vars[1] = 1.3f;

  uint8_t data[maxLength];

  uint32_t  addrOne = __builtin_bswap32((uint32_t) &vars[0]);
  uint32_t  addrTwo = __builtin_bswap32((uint32_t) &vars[1]);

  printf("\n");
  printf("%lu\n", (uint32_t) &vars[0]);
  printf("%lu\n", (uint32_t) &vars[1]);

  char addrBytesOne[5];
  copyByte(addrBytesOne, &addrOne);
  printf("%02x\n", addrBytesOne);

  char addrBytesTwo[5];
  copyByte(addrBytesTwo, &addrTwo);
  printf("%02x\n", addrBytesTwo);


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
  memcpy(data + len, partThree, 69);
  len += 69;

  data[len] = '\0';

  printf("\n");
  for(int i = 0; i < len+1; i++){
    printf("%02x ",data[i]);
  }
  printf("\n");


  ScopeHandle scope = Scope_create(100,2,400,ETHERNET);
  Scope_command(scope, (char*) data, len);
  Scope_poll(scope);

  ChannelHandle channel = Scope_test(scope, 0);
  void* addr = Channel_getPollAddress(channel);

  vars[0] = 1.45f;
  printf("%lf", vars[0]);
  printf("%lf", vars[1]);
}