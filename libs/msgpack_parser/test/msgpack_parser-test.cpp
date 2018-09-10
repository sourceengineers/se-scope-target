#include <gtest/gtest.h>

/*

{
	"transport": "...",
	"payload": {
		"sc_cmd": {
			"cf_running": {
				"1": false,
				"2": true,
				"3": false,
				"4": true,
				"5": true
			},
			"ev_poll": "110",
			"ev_trans": "",
			"cf_addr": {
				"1": 1111,
				"4": 4444,
				"5": 5555
			},
			"cf_tgr": {
				"cl_id": 1,
				"mode": "Continous",
				"level": 2.5,
				"edge": true
			},
			"cf_t_inc": 10
		},
		"flow_ctrl": "ACK"
	}
}
*/

/* msgpack Data is corresbonding to the above Json object */
char data[] = "\x82\xa9\x74\x72\x61\x6e\x73\x70\x6f\x72\x74\xa3\x2e\x2e\x2e\xa7\x70\x61\x79\x6c\x6f\x61\x64\x82\xa6\x73\x63\x5f\x63\x6d\x64\x86\xaa\x63\x66\x5f\x72\x75\x6e\x6e\x69\x6e\x67\x85\xa1\x31\xc2\xa1\x32\xc3\xa1\x33\xc2\xa1\x34\xc3\xa1\x35\xc3\xa7\x65\x76\x5f\x70\x6f\x6c\x6c\xa3\x31\x31\x30\xa8\x65\x76\x5f\x74\x72\x61\x6e\x73\xa0\xa7\x63\x66\x5f\x61\x64\x64\x72\x83\xa1\x31\xcd\x04\x57\xa1\x34\xcd\x11\x5c\xa1\x35\xcd\x15\xb3\xa6\x63\x66\x5f\x74\x67\x72\x84\xa5\x63\x6c\x5f\x69\x64\x01\xa4\x6d\x6f\x64\x65\xa9\x43\x6f\x6e\x74\x69\x6e\x6f\x75\x73\xa5\x6c\x65\x76\x65\x6c\xcb\x40\x04\x00\x00\x00\x00\x00\x00\xa4\x65\x64\x67\x65\xc3\xa8\x63\x66\x5f\x74\x5f\x69\x6e\x63\x0a\xa9\x66\x6c\x6f\x77\x5f\x63\x74\x72\x6c\xa3\x41\x43\x4b\0";

extern "C" {
    #include "msgpack_unpacker.h"
}


TEST(msgpack_unpacker, unpack_cmd_cf_tgr)
{
  Msgpack_unpacker* unpacker = msgpack_unpacker_create();
  unpacker->unpack(unpacker, data, 1000);
  msgpack_object cmd = unpacker->getCmdObj(unpacker, (char *) "cf_tgr");

  int channelId = unpacker->getIntFromObj(cmd, 0);
  EXPECT_EQ(channelId, 1);

  char str[50];
  unpacker->getStringFromObj(cmd, 1, str, 50);
  EXPECT_EQ(str[0], 'C');
  EXPECT_EQ(str[1], 'o');

  unpacker->getStringFromObj(cmd, 1, str, 1);
  EXPECT_EQ(str[0],'\0');


  float level = unpacker->getFloatFromObj(cmd, 2);
  EXPECT_EQ(level, 2.5f);

  bool edge = unpacker->getBoolFromObj(cmd, 3);
  EXPECT_EQ(edge, true);
}


TEST(msgpack_unpacker, unpack_faulty_data)
{
  Msgpack_unpacker* unpacker = msgpack_unpacker_create();

  /* Copy data and manipulate it */
  char str[200];
  strcpy(str, data);
  str[5] = '\0';


  bool resp = unpacker->unpack(unpacker, str, 1000);

  ASSERT_EQ(resp, false);

}
