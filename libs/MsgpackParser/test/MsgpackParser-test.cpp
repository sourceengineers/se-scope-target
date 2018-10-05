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
const char data[] = "\x82\xa9\x74\x72\x61\x6e\x73\x70\x6f\x72\x74\xa3\x2e\x2e\x2e\xa7\x70\x61\x79\x6c\x6f\x61\x64\x82\xa6\x73\x63\x5f\x63\x6d\x64\x86\xaa\x63\x66\x5f\x72\x75\x6e\x6e\x69\x6e\x67\x85\xa1\x31\xc2\xa1\x32\xc3\xa1\x33\xc2\xa1\x34\xc3\xa1\x35\xc3\xa7\x65\x76\x5f\x70\x6f\x6c\x6c\xa3\x31\x31\x30\xa8\x65\x76\x5f\x74\x72\x61\x6e\x73\xa0\xa7\x63\x66\x5f\x61\x64\x64\x72\x83\xa1\x31\xcd\x04\x57\xa1\x34\xcd\x11\x5c\xa1\x35\xcd\x15\xb3\xa6\x63\x66\x5f\x74\x67\x72\x84\xa5\x63\x6c\x5f\x69\x64\x01\xa4\x6d\x6f\x64\x65\xa9\x43\x6f\x6e\x74\x69\x6e\x6f\x75\x73\xa5\x6c\x65\x76\x65\x6c\xcb\x40\x04\x00\x00\x00\x00\x00\x00\xa4\x65\x64\x67\x65\xc3\xa8\x63\x66\x5f\x74\x5f\x69\x6e\x63\x0a\xa9\x66\x6c\x6f\x77\x5f\x63\x74\x72\x6c\xa3\x41\x43\x4b\0";

extern "C" {
    #include "MsgpackUnpacker.h"
}

using namespace testing;


TEST(msgpack_unpacker, unpack_ammount_of_commands)
{
  MsgpackUnpackerHandle unpacker = MsgpackUnpacker_create(1000);
  IUnpackerHandle parser = MsgpackUnpacker_getIUnpacker(unpacker);

  size_t strLength = strlen(data) + 40;
  if(parser->unpack(parser, data, strLength)){
    parser->activateNewMessage(parser);
  }

  size_t numberOfCommands = parser->getNumberOfCommands(parser);

  EXPECT_EQ(numberOfCommands, 6);
}


TEST(msgpack_unpacker, unpack_name_of_command)
{
  MsgpackUnpackerHandle unpacker = MsgpackUnpacker_create(1000);
  IUnpackerHandle parser = MsgpackUnpacker_getIUnpacker(unpacker);

  size_t strLength = strlen(data) + 40;
  if(parser->unpack(parser, data, strLength)){
    parser->activateNewMessage(parser);
  }

  char name[20];

  parser->getNameOfCommand(parser, name, 20, 0);
  EXPECT_STREQ(name, "cf_running");
  parser->getNameOfCommand(parser, name, 20, 1);
  EXPECT_STREQ(name, "ev_poll");
  parser->getNameOfCommand(parser, name, 20, 2);
  EXPECT_STREQ(name, "ev_trans");
  parser->getNameOfCommand(parser, name, 20, 3);
  EXPECT_STREQ(name, "cf_addr");
  parser->getNameOfCommand(parser, name, 20, 4);
  EXPECT_STREQ(name, "cf_tgr");
  parser->getNameOfCommand(parser, name, 20, 5);
  EXPECT_STREQ(name, "cf_t_inc");

}


TEST(msgpack_unpacker, test_fetcher_functions)
{
  MsgpackUnpackerHandle unpacker = MsgpackUnpacker_create(1000);
  IUnpackerHandle parser = MsgpackUnpacker_getIUnpacker(unpacker);

  size_t strLength = strlen(data) + 40;
  if(parser->unpack(parser, data, strLength)){
    parser->activateNewMessage(parser);
  }


  char fieldName[20];
  parser->getNameOfField(parser, (const char*) "cf_running", fieldName, 20, 1);
  EXPECT_STREQ(fieldName, "2");

  int mapSize = parser->getNumberOfFields(parser, (const char*) "cf_tgr");
  EXPECT_EQ(mapSize, 4);

  mapSize = parser->getNumberOfFields(parser, (const char*) "cf_running");
  EXPECT_EQ(mapSize, 5);

  int channelId = parser->getIntFromCommand(parser, (const char*) "cf_tgr", (const char*) "cl_id");
  EXPECT_EQ(channelId, 1);

  char fetchedData[20];
  parser->getStringFromCommand(parser,(const char*) "cf_tgr", (const char*)  "mode", fetchedData, 20);
  EXPECT_STREQ(fetchedData, "Continous");

  float level = parser->getFloatFromCommand(parser, (const char*) "cf_tgr", "level");
  EXPECT_EQ(level, 2.5f);

  bool edge = parser->getBoolFromCommand(parser, (const char*) "cf_tgr", "edge");
  EXPECT_EQ(edge, true);

  int stamp = parser->getIntFromCommand(parser, (const char*) "cf_t_inc", "");
  EXPECT_EQ(stamp, 10);
}


TEST(msgpack_unpacker, test_automatic_mode)
{
  MsgpackUnpackerHandle unpacker = MsgpackUnpacker_create(1000);
  IUnpackerHandle parser = MsgpackUnpacker_getIUnpacker(unpacker);

  size_t strLength = strlen(data) + 40;
  if(parser->unpack(parser, data, strLength)){
    parser->activateNewMessage(parser);
  }

  char fieldName[20];
  char commandName[20];

  size_t numberOfCommands = parser->getNumberOfCommands(parser);
  EXPECT_EQ(numberOfCommands, 6);

  parser->getNameOfCommand(parser,commandName, 20, 0);
  EXPECT_STREQ(commandName, "cf_running");

  size_t numberOfFields = parser->getNumberOfFields(parser, (const char*) commandName);
  EXPECT_EQ(numberOfFields, 5);

  parser->getNameOfField(parser, (const char* ) commandName, fieldName, 20, 0);
  EXPECT_STREQ(fieldName, "1");

  /* For the next step, the command parser has to know which values to parse. */
  bool setRunning = parser->getBoolFromCommand(parser, commandName, fieldName);
  EXPECT_EQ(setRunning, false);

  setRunning = parser->getBoolFromCommand(parser, commandName, (const char*) "2");
  EXPECT_EQ(setRunning, true);
}

TEST(msgpack_unpacker, test_errors)
{
  MsgpackUnpackerHandle unpacker = MsgpackUnpacker_create(1000);
  IUnpackerHandle parser = MsgpackUnpacker_getIUnpacker(unpacker);

  size_t strLength = 182;
  char faultyData[strLength + 1];
  memcpy(faultyData, data, strLength);

  faultyData[10] = 'a';
  faultyData[11] = '\0';
  faultyData[12] = '1';

  bool result;
  if((result = parser->unpack(parser, faultyData, strLength))){
    parser->activateNewMessage(parser);
  }

  EXPECT_EQ(result, false);
  float level = parser->getFloatFromCommand(parser, (const char*) "cf_tgr", "level");
  EXPECT_EQ(level, 0.0f);

  if((result = parser->unpack(parser, data, strLength))){
    parser->activateNewMessage(parser);
  }

  EXPECT_EQ(result, true);
  level = parser->getFloatFromCommand(parser, (const char*) "cf_tgr", "level");
  EXPECT_EQ(level, 2.5f);

  if((result = parser->unpack(parser, faultyData, strLength))){
    parser->activateNewMessage(parser);
  }

  EXPECT_EQ(result, false);
  level = parser->getFloatFromCommand(parser, (const char*) "cf_tgr", "level");
  EXPECT_EQ(level, 2.5f);

  if((result = parser->unpack(parser, faultyData, strLength))){
    parser->activateNewMessage(parser);
  }

  EXPECT_EQ(result, false);

  if((result = parser->unpack(parser, data, strLength))){
    parser->activateNewMessage(parser);
  }

  EXPECT_EQ(result, true);

  if((result = parser->unpack(parser, data, strLength))){
    parser->activateNewMessage(parser);
  }

  EXPECT_EQ(result, true);


  level = parser->getFloatFromCommand(parser, (const char*) "cf_tgr", "lavel");
  EXPECT_EQ(level, 0.0f);

  level = parser->getFloatFromCommand(parser, (const char*) "cf_tgri", "lavel");
  EXPECT_EQ(level, 0.0f);

}