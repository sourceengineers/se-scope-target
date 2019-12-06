#include <gtest/gtest.h>

extern "C" {
#include <Scope/Serialisation/JsonParser/JsonUnpacker.h>
}

using namespace testing;
using namespace std;


static const char* validInput = (const char*) ""
                                        "{"
                                            "\"ev_announce\": null,"
                                            "\"ev_trans\": null,"
                                            "\"cf_addr\": {"
                                                "\"cl_id_1\": [111, \"SE_UINT8\"],"
                                                "\"cl_id_2\": [222,\"SE_UINT16\"],"
                                                "\"cl_id_3\": [333,\"SE_FLOAT\"]"
                                            "},"
                                            "\"cf_tgr\": {"
                                                "\"cl_id\": 15,"
                                                "\"mode\": \"Continous\","
                                                "\"level\": 1.75,"
                                                "\"edge\": \"fallig\""
                                            "}"
                                        "}";


class JsonUnpackerTest : public ::testing::Test{
#define INPUT_BUFFER_SIZE (400)

protected:
    JsonUnpackerTest()
            : Test(){
    }

    void SetUp() override{
        _input = BufferedByteStream_create(INPUT_BUFFER_SIZE);
        _iinput = BufferedByteStream_getIByteStream(_input);
        _unpacker = JsonUnpacker_create(_iinput);
        _iUnpacker = JsonUnpacker_getIUnpacker(_unpacker);
    }

    void TearDown() override{
        BufferedByteStream_destroy(_input);
        JsonUnpacker_destroy(_unpacker);
    }

    ~JsonUnpackerTest() override{
    }

    BufferedByteStreamHandle _input;
    IByteStreamHandle _iinput;
    JsonUnpackerHandle _unpacker;
    IUnpackerHandle _iUnpacker;
};

TEST_F(JsonUnpackerTest, values_from_commands){
    _iinput->write(_iinput, (uint8_t*) validInput, strlen(validInput));
    _iinput->writeByte(_iinput, '\0');
    bool unpackSuccessful = _iUnpacker->unpack(_iUnpacker, CF_TRIGGER);

    CommandFetchingInformation info = {
            .commandName = (char*) "cf_tgr",
            .fieldName = (char*) "cl_id",
            .isInArray = false,
            .arrayIndex = 0
    };

    int data = _iUnpacker->getIntFromCommand(_iUnpacker, &info);
    EXPECT_EQ(15, data);

    info.fieldName = (char*) "level";

    float fdata = _iUnpacker->getFloatFromCommand(_iUnpacker, &info);
    EXPECT_EQ(1.75, fdata);

    char mode[20];
    info.fieldName = (char*) "mode";
    _iUnpacker->getStringFromCommand(_iUnpacker, &info, mode, 20);
    EXPECT_STREQ(mode, "Continous");

    int numberOfFields;
    numberOfFields = _iUnpacker->getNumberOfFields(_iUnpacker, (const char*) "cf_addr");
    EXPECT_EQ(numberOfFields, 3);

    char nameOfField[30];
    _iUnpacker->getNameOfField(_iUnpacker, (const char*) "cf_tgr", nameOfField, 30, 0);
    EXPECT_STREQ(nameOfField, "cl_id");
    _iUnpacker->getNameOfField(_iUnpacker, (const char*) "cf_tgr", nameOfField, 30, 1);
    EXPECT_STREQ(nameOfField, "mode");
    _iUnpacker->getNameOfField(_iUnpacker, (const char*) "cf_tgr", nameOfField, 30, 2);
    EXPECT_STREQ(nameOfField, "level");
    _iUnpacker->getNameOfField(_iUnpacker, (const char*) "cf_tgr", nameOfField, 30, 3);
    EXPECT_STREQ(nameOfField, "edge");
}

TEST_F(JsonUnpackerTest, values_from_array){

    _iinput->write(_iinput, (uint8_t*) validInput, strlen(validInput));
    _iinput->writeByte(_iinput, '\0');

    bool unpackSuccessful = _iUnpacker->unpack(_iUnpacker, CF_ADDR);

    CommandFetchingInformation info = {
            .commandName = (char*) "cf_addr",
            .fieldName = (char*) "cl_id_1",
            .isInArray = true,
            .arrayIndex = 0
    };

    int data = _iUnpacker->getIntFromCommand(_iUnpacker, &info);
    EXPECT_EQ(111, data);

    info.fieldName = (char*) "cl_id_2";
    info.arrayIndex = 1;

    char dataType[20];
    _iUnpacker->getStringFromCommand(_iUnpacker, &info, dataType, 20);
    EXPECT_STREQ(dataType, "SE_UINT16");

}

static const char* faultyData = (const char*)
                                              "{"
                                                "\"ev_announce\": null,"
                                                "\"ev_trans\": null,"
                                                "\"cf_addr\": {"
                                                    "\"cl_id_1\": [\"111\", \"SE_UINT8\"],"
                                                    "\"cl_id_2\": [222,\"SE_UINT16\"],"
                                                    "\"cl_id_3\": [333,\"SE_FLOAT\"]"
                                                "},"
                                                "\"cf_tgr\": {"
                                                    "\"cl_\": 15,"
                                                    "\"mode\": \"Continous\","
                                                    "\"level\": 1.75,"
                                                    "\"edge\": \"fallig\""
                                                "}"
                                              "}";


static const char* notParsingData = (const char*)
                                                  "\"ev_announce\": null,"
                                                  "\"ev_trans\": null,"
                                                  "\"cf_addr\": {"
                                                    "\"cl_id_1\": [\"111\", \"SE_UINT8\"],"
                                                    "\"cl_id_2\": [222,\"SE_UINT16\"],"
                                                    "\"cl_id_3\": [333,\"SE_FLOAT\"]"
                                                  "},"
                                                  "\"cf_tgr\": {"
                                                    "\"cl_\": 15,"
                                                    "\"mode\": \"Continous\","
                                                    "\"level\": 1.75,"
                                                    "\"edge\": \"falig\""
                                                  "}";

TEST_F(JsonUnpackerTest, edge_cases){
    _iinput->write(_iinput, (uint8_t*) notParsingData, strlen(notParsingData));
    _iinput->writeByte(_iinput, '\0');
    bool unpackSuccessful = _iUnpacker->unpack(_iUnpacker, CF_TRIGGER);

    ASSERT_EQ(unpackSuccessful, false);

    _iinput->write(_iinput, (uint8_t*) faultyData, strlen(faultyData));
    _iinput->writeByte(_iinput, '\0');
    _iUnpacker->unpack(_iUnpacker, CF_TRIGGER);

    CommandFetchingInformation info = {
            .commandName = (char*) "cf_tgr",
            .fieldName = (char*) "cl_id",
            .isInArray = false,
            .arrayIndex = 0
    };

    int res = (int) _iUnpacker->getIntFromCommand(_iUnpacker, &info);
    EXPECT_EQ(0, res);

    info.commandName = (char*) "cf_addr";
    info.fieldName = (char*) "cl_id_1";
    info.isInArray = true;
    info.arrayIndex = 0;
    res = _iUnpacker->getIntFromCommand(_iUnpacker, &info);
    EXPECT_EQ(0, res);

    info.commandName = (char*) "cf_tgr";
    info.fieldName = (char*) "level";
    info.isInArray = false;
    info.arrayIndex = 0;

    res = _iUnpacker->getIntFromCommand(_iUnpacker, &info);
    EXPECT_EQ(1, res);

    info.commandName = (char*) "cf_addr";
    info.fieldName = (char*) "cl_id_1";
    info.isInArray = true;
    info.arrayIndex = 2;

    res = _iUnpacker->getIntFromCommand(_iUnpacker, &info);
    EXPECT_EQ(0, res);
}