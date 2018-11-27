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
                                        "\"cl_id_1\": ["
                                        "111,"
                                        "\"UINT8\""
                                        "],"
                                        "\"cl_id_2\": ["
                                        "222,"
                                        "\"UINT16\""
                                        "],"
                                        "\"cl_id_n\": ["
                                        "333,"
                                        "\"FLOAT\""
                                        "]"
                                        "},"
                                        "\"cf_tgr\": {"
                                        "\"cl_id\": 0,"
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

}
