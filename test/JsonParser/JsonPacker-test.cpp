#include <gtest/gtest.h>

extern "C" {
    #include <Scope/JsonParser/JsonUnpacker.h>
}

using namespace testing;
using namespace std;


static const char* data = (const char*) "{\"transport\": \"...\","
                                          "\"payload\": {"
                                            "\"sc_cmd\": {"
                                              "\"ev_announce\": null,"
                                              "\"ev_trans\": null,"
                                              "\"cf_addr\": {"
                                                "\"cl_id_1\": [111, \"UINT8\"],"
                                                "\"cl_id_2\": [222,\"UINT16\"],"
                                                "\"cl_id_3\": [333,\"FLOAT\"]"
                                              "},"
                                              "\"cf_tgr\": {"
                                                "\"cl_id\": 15,"
                                                "\"mode\": \"Continous\","
                                                "\"level\": 1.75,"
                                                "\"edge\": \"fallig\""
                                              "}"
                                            "}"
                                          "}"
                                        "}";

TEST(json_packer, unpack_test)
{
  IUnpackerHandle unpacker = JsonUnpacker_getIUnpacker(JsonUnpacker_create(400));

  bool unpackSuccessful = unpacker->unpack(unpacker, data, strlen(data));

  EXPECT_EQ(unpackSuccessful, true);
  unpacker->activateNewMessage(unpacker);

  int numberOfCommands = unpacker->getNumberOfCommands(unpacker);

  EXPECT_EQ(numberOfCommands, 4);
}

TEST(json_packer, command_name)
{
  IUnpackerHandle unpacker = JsonUnpacker_getIUnpacker(JsonUnpacker_create(400));

  bool unpackSuccessful = unpacker->unpack(unpacker, data, strlen(data));

  unpacker->activateNewMessage(unpacker);

  char name[20];
  unpacker->getNameOfCommand(unpacker, name, 20, 0);
  EXPECT_STREQ(name, "ev_announce");
  unpacker->getNameOfCommand(unpacker, name, 20, 1);
  EXPECT_STREQ(name, "ev_trans");
  unpacker->getNameOfCommand(unpacker, name, 20, 2);
  EXPECT_STREQ(name, "cf_addr");
  unpacker->getNameOfCommand(unpacker, name, 20, 3);
  EXPECT_STREQ(name, "cf_tgr");

}

TEST(json_packer, values_from_commands)
{
  IUnpackerHandle unpacker = JsonUnpacker_getIUnpacker(JsonUnpacker_create(400));

  bool unpackSuccessful = unpacker->unpack(unpacker, data, strlen(data));

  unpacker->activateNewMessage(unpacker);

  CommandFetchingInformation info = {
      .commandName = (char*) "cf_tgr",
      .fieldName = (char*)"cl_id",
      .isInArray = false,
      .arrayIndex = 0
  };

  int data = unpacker->getIntFromCommand(unpacker, &info);
  EXPECT_EQ(15, data);

  info.fieldName = (char*)"level";


  float fdata = unpacker->getIntFromCommand(unpacker, &info);
  EXPECT_EQ(1.75, fdata);

}