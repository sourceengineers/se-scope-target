#include <gtest/gtest.h>

extern "C" {
    #include <Scope/Parser/JsonParser/JsonUnpacker.h>
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

TEST(json_unpacker, unpack_test)
{
  IUnpackerHandle unpacker = JsonUnpacker_getIUnpacker(JsonUnpacker_create());

  bool unpackSuccessful = unpacker->unpack(unpacker, data, strlen(data));

  EXPECT_EQ(unpackSuccessful, true);
  unpacker->activateNewMessage(unpacker);

  int numberOfCommands = unpacker->getNumberOfCommands(unpacker);

  EXPECT_EQ(numberOfCommands, 4);
}

TEST(json_unpacker, command_name)
{
  IUnpackerHandle unpacker = JsonUnpacker_getIUnpacker(JsonUnpacker_create());

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

TEST(json_unpacker, values_from_commands)
{
  IUnpackerHandle unpacker = JsonUnpacker_getIUnpacker(JsonUnpacker_create());

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

  float fdata = unpacker->getFloatFromCommand(unpacker, &info);
  EXPECT_EQ(1.75, fdata);

  char mode[20];
  info.fieldName = (char*)"mode";
  unpacker->getStringFromCommand(unpacker, &info, mode, 20);
  EXPECT_STREQ(mode, "Continous");

  int numberOfFields;
  numberOfFields = unpacker->getNumberOfFields(unpacker, (const char*) "cf_addr");
  EXPECT_EQ(numberOfFields, 3);

  char nameOfField[30];
  unpacker->getNameOfField(unpacker, (const char*) "cf_tgr", nameOfField, 30, 0);
  EXPECT_STREQ(nameOfField, "cl_id");
  unpacker->getNameOfField(unpacker, (const char*) "cf_tgr", nameOfField, 30, 1);
  EXPECT_STREQ(nameOfField, "mode");
  unpacker->getNameOfField(unpacker, (const char*) "cf_tgr", nameOfField, 30, 2);
  EXPECT_STREQ(nameOfField, "level");
  unpacker->getNameOfField(unpacker, (const char*) "cf_tgr", nameOfField, 30, 3);
  EXPECT_STREQ(nameOfField, "edge");
}

TEST(json_unpacker, values_from_array)
{
  IUnpackerHandle unpacker = JsonUnpacker_getIUnpacker(JsonUnpacker_create());

  bool unpackSuccessful = unpacker->unpack(unpacker, data, strlen(data));

  unpacker->activateNewMessage(unpacker);

  CommandFetchingInformation info = {
          .commandName = (char*) "cf_addr",
          .fieldName = (char*)"cl_id_1",
          .isInArray = true,
          .arrayIndex = 0
  };

  int data = unpacker->getIntFromCommand(unpacker, &info);
  EXPECT_EQ(111, data);

  info.fieldName = (char*)"cl_id_2";
  info.arrayIndex = 1;

  char dataType[20];
  unpacker->getStringFromCommand(unpacker, &info, dataType, 20);
  EXPECT_STREQ(dataType, "UINT16");

}

TEST(json_unpacker, transport_field)
{
  IUnpackerHandle unpacker = JsonUnpacker_getIUnpacker(JsonUnpacker_create());

  bool unpackSuccessful = unpacker->unpack(unpacker, data, strlen(data));

  unpacker->activateNewMessage(unpacker);

  size_t lengthCheck = unpacker->getLengthOfCheck(unpacker);
  EXPECT_EQ(lengthCheck, 3);

  size_t lengthOfDataToCheck = unpacker->getLengthOfBytesToCheck(unpacker);
  EXPECT_EQ(lengthOfDataToCheck, 215);

  uint8_t dataToCheck[300];
  unpacker->getBytesToCheck(unpacker, dataToCheck);

  EXPECT_STREQ((char*) dataToCheck, "{\"sc_cmd\": {\"ev_announce\": null,\"ev_trans\": null,\"cf_addr\": {\"cl_id_1\": [111, \"UINT8\"],\"cl_id_2\": [222,\"UINT16\"],\"cl_id_3\": [333,\"FLOAT\"]},\"cf_tgr\": {\"cl_id\": 15,\"mode\": \"Continous\",\"level\": 1.75,\"edge\": \"fallig\"}}}");

  uint8_t checkData[30];
  unpacker->getCheck(unpacker, checkData);

  EXPECT_STREQ((char*) checkData, "...");

}

static const char* faultyData = (const char*)  "{\"transport\": \"...\","
                                               "\"payload\": {"
                                               "\"sc_cmd\": {"
                                               "\"ev_announce\": null,"
                                               "\"ev_trans\": null,"
                                               "\"cf_addr\": {"
                                               "\"cl_id_1\": [\"111\", \"UINT8\"],"
                                               "\"cl_id_2\": [222,\"UINT16\"],"
                                               "\"cl_id_3\": [333,\"FLOAT\"]"
                                               "},"
                                               "\"cf_tgr\": {"
                                               "\"cl_\": 15,"
                                               "\"mode\": \"Continous\","
                                               "\"level\": 1.75,"
                                               "\"edge\": \"fallig\""
                                               "}"
                                               "}"
                                               "}"
                                               "}";


static const char* notParsingData = (const char*) "\"transport\": \"...\","
                                               "\"payload\": {"
                                               "\"sc_cmd\": {"
                                               "\"ev_announce\": null,"
                                               "\"ev_trans\": null,"
                                               "\"cf_addr\": {"
                                               "\"cl_id_1\": [\"111\", \"UINT8\"],"
                                               "\"cl_id_2\": [222,\"UINT16\"],"
                                               "\"cl_id_3\": [333,\"FLOAT\"]"
                                               "},"
                                               "\"cf_tgr\": {"
                                               "\"cl_\": 15,"
                                               "\"mode\": \"Continous\","
                                               "\"level\": 1.75,"
                                               "\"edge\": \"falig\""
                                               "}"
                                               "}"
                                               "}"
                                               "}";


TEST(json_unpacker, edge_cases)
{
  IUnpackerHandle unpacker = JsonUnpacker_getIUnpacker(JsonUnpacker_create());

  /* Check that faulty messages not get parsed */
  bool unpackSuccessful = unpacker->unpack(unpacker, notParsingData, strlen(notParsingData));

  ASSERT_EQ(unpackSuccessful, false);

  unpacker->unpack(unpacker, faultyData, strlen(faultyData));
  unpacker->activateNewMessage(unpacker);

  /* Check if non matching field names don't get parsed */
  CommandFetchingInformation info = {
          .commandName = (char*) "cf_tgr",
          .fieldName = (char*)"cl_id",
          .isInArray = false,
          .arrayIndex = 0
  };

  int res = (int) unpacker->getIntFromCommand(unpacker, &info);
  EXPECT_EQ(0, res);

  /* Check if int doesn't accidentally parse string */
  info.commandName = (char*) "cf_addr";
  info.fieldName = (char*) "cl_id_1";
  info.isInArray = true;
  info.arrayIndex = 0;
  res = unpacker->getIntFromCommand(unpacker, &info);
  EXPECT_EQ(0, res);

  info.commandName = (char*) "cf_tgr";
  info.fieldName = (char*) "level";
  info.isInArray = false;
  info.arrayIndex = 0;

  /* Check if int parses float without crashing */
  res = unpacker->getIntFromCommand(unpacker, &info);
  EXPECT_EQ(1, res);

  /* Check if overflows in arrays are prevented */
  info.commandName = (char*) "cf_addr";
  info.fieldName = (char*) "cl_id_1";
  info.isInArray = true;
  info.arrayIndex = 2;

  res = unpacker->getIntFromCommand(unpacker, &info);
  EXPECT_EQ(0, res);
}