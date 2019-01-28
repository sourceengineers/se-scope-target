#include <gtest/gtest.h>

extern "C" {
#include <Scope/Serialisation/JsonParser/JsonUnpacker.h>
}

using namespace testing;
using namespace std;


static const char* data = (const char*) "{\"payload\": {"
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

TEST(json_unpacker, unpack_test){

    IByteStreamHandle stream = BufferedByteStream_getIByteStream(BufferedByteStream_create(400));
    stream->write(stream,(uint8_t*) data, strlen(data));
    IUnpackerHandle unpacker = JsonUnpacker_getIUnpacker(JsonUnpacker_create(stream));
    bool unpackSuccessful = unpacker->unpack(unpacker);

    EXPECT_EQ(unpackSuccessful, true);

    int numberOfCommands = unpacker->getNumberOfCommands(unpacker);

    EXPECT_EQ(numberOfCommands, 4);
}

TEST(json_unpacker, command_name){

    IByteStreamHandle stream = BufferedByteStream_getIByteStream(BufferedByteStream_create(400));
    stream->write(stream,(uint8_t*) data, strlen(data));
    IUnpackerHandle unpacker = JsonUnpacker_getIUnpacker(JsonUnpacker_create(stream));
    bool unpackSuccessful = unpacker->unpack(unpacker);

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

TEST(json_unpacker, values_from_commands){
    IByteStreamHandle stream = BufferedByteStream_getIByteStream(BufferedByteStream_create(400));
    stream->write(stream,(uint8_t*) data, strlen(data));
    IUnpackerHandle unpacker = JsonUnpacker_getIUnpacker(JsonUnpacker_create(stream));
    bool unpackSuccessful = unpacker->unpack(unpacker);


    CommandFetchingInformation info = {
            .commandName = (char*) "cf_tgr",
            .fieldName = (char*) "cl_id",
            .isInArray = false,
            .arrayIndex = 0
    };

    int data = unpacker->getIntFromCommand(unpacker, &info);
    EXPECT_EQ(15, data);

    info.fieldName = (char*) "level";

    float fdata = unpacker->getFloatFromCommand(unpacker, &info);
    EXPECT_EQ(1.75, fdata);

    char mode[20];
    info.fieldName = (char*) "mode";
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

TEST(json_unpacker, values_from_array){
    IByteStreamHandle stream = BufferedByteStream_getIByteStream(BufferedByteStream_create(400));
    stream->write(stream,(uint8_t*) data, strlen(data));
    IUnpackerHandle unpacker = JsonUnpacker_getIUnpacker(JsonUnpacker_create(stream));
    bool unpackSuccessful = unpacker->unpack(unpacker);

    CommandFetchingInformation info = {
            .commandName = (char*) "cf_addr",
            .fieldName = (char*) "cl_id_1",
            .isInArray = true,
            .arrayIndex = 0
    };

    int data = unpacker->getIntFromCommand(unpacker, &info);
    EXPECT_EQ(111, data);

    info.fieldName = (char*) "cl_id_2";
    info.arrayIndex = 1;

    char dataType[20];
    unpacker->getStringFromCommand(unpacker, &info, dataType, 20);
    EXPECT_STREQ(dataType, "UINT16");

}

static const char* faultyData = (const char*) "\"payload\": {"
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
                                              "}";


static const char* notParsingData = (const char*) "\"payload\": {"
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
                                                  "}";

TEST(json_unpacker, edge_cases){
    IByteStreamHandle stream = BufferedByteStream_getIByteStream(BufferedByteStream_create(400));
    stream->write(stream,(uint8_t*) notParsingData, strlen(notParsingData));
    IUnpackerHandle unpacker = JsonUnpacker_getIUnpacker(JsonUnpacker_create(stream));

    bool unpackSuccessful = unpacker->unpack(unpacker);

    ASSERT_EQ(unpackSuccessful, false);

    stream->write(stream,(uint8_t*) faultyData, strlen(faultyData));
    unpacker->unpack(unpacker);

    CommandFetchingInformation info = {
            .commandName = (char*) "cf_tgr",
            .fieldName = (char*) "cl_id",
            .isInArray = false,
            .arrayIndex = 0
    };

    int res = (int) unpacker->getIntFromCommand(unpacker, &info);
    EXPECT_EQ(0, res);

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

    res = unpacker->getIntFromCommand(unpacker, &info);
    EXPECT_EQ(1, res);

    info.commandName = (char*) "cf_addr";
    info.fieldName = (char*) "cl_id_1";
    info.isInArray = true;
    info.arrayIndex = 2;

    res = unpacker->getIntFromCommand(unpacker, &info);
    EXPECT_EQ(0, res);
}