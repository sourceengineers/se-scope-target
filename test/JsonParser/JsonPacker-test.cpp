#include <gtest/gtest.h>

extern "C" {
    #include <Scope/JsonParser/JsonPacker.h>
    #include <Scope/GeneralPurpose/ByteStream.h>
    #include <Scope/GeneralPurpose/IntStream.h>
    #include <Scope/GeneralPurpose/FloatStream.h>
}

using namespace testing;
using namespace std;

IComValidator validator;

static bool validateCheck(IComValidatorHandle self, const uint8_t* check, const size_t lengthOfCheck,
                      const uint8_t* bytesToCheck, const size_t lengthOfBytesToCheck){
  return true;
}

static size_t getCheckLength(IComValidatorHandle self){
  return 4;
}

static void createCheck(IComValidatorHandle self, uint8_t* checksum, const uint8_t* bytesToCheck, const size_t length){

  for (int i = 0; i < length; ++i) {
    checksum[i%4] += bytesToCheck[i];
  }
}


static bool checkPresentInProtocol(IComValidatorHandle self){
  return true;
}


TEST(json_packer, unpack_test){

  validator.validateCheck = &validateCheck;
  validator.getCheckLength = &getCheckLength;
  validator.createCheck = &createCheck;
  validator.checkPresentInProtocol = &checkPresentInProtocol;

  size_t channelSize = 400;
  OutputBufferSizes sizes = JsonPacker_calculateBufferSizes(5, 10, channelSize);

  IByteStreamHandle outputStream = ByteStream_getIByteStream(ByteStream_create(sizes.outputBufferSize));

  IIntStreamHandle timestamp = IntStream_getIIntStream(IntStream_create(channelSize));
  IFloatStreamHandle ch1 = FloatStream_getIFloatStream(FloatStream_create(channelSize));
  IFloatStreamHandle ch2 = FloatStream_getIFloatStream(FloatStream_create(channelSize));

  IPackerHandle packer = JsonPacker_getIPacker(JsonPacker_create(sizes, &validator, outputStream));

  packer->prepareAddressAnnouncement(packer, "VAR_1", "UINT32", 11111);
  packer->prepareAddressAnnouncement(packer, "VAR_2", "FLOAT", 22222);

  packer->prepareTrigger(packer, true, 1, 100);

  packer->prepareTimeIncrement(packer, 10);
  packer->prepareFlowControl(packer, "ACK");

  for (int i = 0; i < 5; ++i) {
    timestamp->writeData(timestamp, i);
    ch1->writeData(ch1, (float) i);
    ch2->writeData(ch2, (float) i);
  }

  packer->prepareTimestamp(packer, timestamp);
  packer->prepareChannel(packer, ch1, 0);
  packer->prepareChannel(packer, ch2, 1);

  packer->pack(packer);

  size_t dataPending = outputStream->length(outputStream);
  char data[dataPending];

  outputStream->read(outputStream, (uint8_t*) data, dataPending);

  printf("Output: %s", data);
}
