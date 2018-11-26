/*!****************************************************************************************************************************************
 * @file         MsgpackPacker.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/MsgpackParser/MsgpackPacker.h>
#include <msgpack/object.h>
#include <msgpack.h>
#include <string.h>
#include <stdio.h>
#include <Scope/GeneralPurpose/Memory.h>
#include <Scope/MsgpackParser/MsgpackCommon.h>


static const char* KEYWORD_PAYLOAD = "payload";
static const char* KEYWORD_TRANSPORT = "transport";
static const char* KEYWORD_SC_DATA = "sc_data";
static const char* KEYWORD_PLACEHOLDER = "...";
static const char* KEYWORD_ANNOUNCE = "sc_announce";
static const char* KEYWORD_FLOW_CTRL = "flow_ctrl";
static const char* KEYWORD_CL_DATA = "cl_data";
static const char* KEYWORD_T_STMP = "t_stmp";
static const char* KEYWORD_T_INC = "t_inc";
static const char* KEYWORD_TGR = "tgr";
static const char* KEYWORD_TGR_FOUND = "found";
static const char* KEYWORD_TGR_CL_DATA_IND = "cl_data_ind";
static const char* KEYWORD_TGR_CL_ID = "cl_id";
static const char* KEYWORD_NUMBER_OF_CHANNELS = "cl_amount";

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __MsgpackPackerPrivateData
{
  size_t maxNumberOfChannels;

  IPacker iPacker;
  msgpack_sbuffer sbuf; /* buffer */
  msgpack_packer pk;    /* packer */

  /* Data for the Announce address feature */
  size_t maxAddressesToAnnounce;
  char** namesOfAddresses;
  char** typesOfAddresses;
  gemmi_uint* addresses;
  gemmi_uint numberOfAddressesToAnnounce;
  bool addressesArePrepared;



    /* Packer and buffer for payload
   * This is used, so the needed data can be fed to the IComValidator interface
   * without having to pack and unpack the data */
  IComValidatorHandle validator;
  msgpack_sbuffer sbufPayload;
  msgpack_packer pkPayload;

  gemmi_uint scDataFields;
  gemmi_uint payloadFields;

  IByteStreamHandle byteStream;


  /* Channel preparation data */
  bool channelsArePrepared;
  size_t numberOfChannelsToSend;
  gemmi_uint* channelIds;

  /* Trigger preparation data */
  bool triggerIsPrepared;
  bool isTriggered;
  gemmi_uint triggerTimestamp;
  gemmi_uint activeChannelId;
  IFloatStreamHandle* floatStreams;

  /* Timestamp preparation data */
  bool timestampIsPrepared;
  IIntStreamHandle currentTimestamp;

  /* Timestamp increment preparation data */
  bool timestampIncrementIsPrepared;
  gemmi_uint timestampIncrement;

  /* Flow Control preparation data */
  bool flowControlIsPrepared;
  char flowcontrol[4];

  /* transport data preparation data */
  bool transportDataIsPrepared;

} MsgpackPackerPrivateData ;

/******************************************************************************
 Private functions
******************************************************************************/

static void incrementPayloadField(MsgpackPackerHandle self){

  if((self->channelsArePrepared == false) && (self->triggerIsPrepared == false) &&
          (self->timestampIsPrepared == false) && (self->timestampIncrementIsPrepared == false) &&
          (self->addressesArePrepared == false)){
    self->payloadFields += 1;
  }
}

static void incrementScDataField(MsgpackPackerHandle self){

  if((self->channelsArePrepared == false) && (self->triggerIsPrepared == false) &&
     (self->timestampIsPrepared == false) && (self->timestampIncrementIsPrepared == false) &&
     (self->addressesArePrepared == false)){
    self->scDataFields += 1;
  }
}

static void prepareChannel(IPackerHandle iPacker, IFloatStreamHandle stream, const gemmi_uint channelId){
  MsgpackPackerHandle self = (MsgpackPackerHandle) iPacker->implementer;

  if(channelId >= self->maxNumberOfChannels){
    return;
  }

  if(self->numberOfChannelsToSend >= self->maxNumberOfChannels){
    return;
  }

  self->channelIds[self->numberOfChannelsToSend] = channelId;
  self->floatStreams[self->numberOfChannelsToSend] = stream;

  self->numberOfChannelsToSend++;

  if(self->channelsArePrepared == true){
    return;
  }

  incrementPayloadField(self);
  self->scDataFields += 1;
  self->channelsArePrepared = true;
}

static void prepareTimeIncrement(IPackerHandle iPacker, const gemmi_uint timeIncrement){
  MsgpackPackerHandle self = (MsgpackPackerHandle) iPacker->implementer;

  if(self->timestampIncrementIsPrepared == true){
    return;
  }

  self->timestampIncrement = timeIncrement;
  incrementPayloadField(self);
  self->scDataFields += 1;
  self->timestampIncrementIsPrepared = true;
}

static void prepareTimestamp(IPackerHandle iPacker, IIntStreamHandle timestamp){
  MsgpackPackerHandle self = (MsgpackPackerHandle) iPacker->implementer;

  if(self->timestampIsPrepared == true){
    return;
  }

  self->currentTimestamp = timestamp;
  incrementPayloadField(self);
  self->scDataFields += 1;
  self->timestampIsPrepared = true;
}

static void prepareTrigger(IPackerHandle iPacker, const bool isTriggered, const gemmi_uint channelId, const gemmi_uint timestamp){
  MsgpackPackerHandle self = (MsgpackPackerHandle) iPacker->implementer;

  if(self->triggerIsPrepared == true){
    return;
  }

  self->isTriggered = isTriggered;
  self->activeChannelId = channelId;
  self->triggerTimestamp = timestamp;
  incrementPayloadField(self);
  self->scDataFields += 1;
  self->triggerIsPrepared = true;
}

static void prepareFlowControl(IPackerHandle iPacker, const char* flowControl){
  MsgpackPackerHandle self = (MsgpackPackerHandle) iPacker->implementer;

  if(self->flowControlIsPrepared == true){
    return;
  }

  /* Flow control data will be "NAK" or "ACK" which is why longer strings will be rejected by default */
  if(strlen(flowControl) > 3){
    return;
  }

  copyString(self->flowcontrol, flowControl, strlen(flowControl));
  self->flowControlIsPrepared = true;
  self->scDataFields += 1;
  self->payloadFields += 1;

}

void prepareAddressAnnouncement(IPackerHandle iPacker, const char* name, const char* type, const gemmi_uint address){
  MsgpackPackerHandle self = (MsgpackPackerHandle) iPacker->implementer;

  if(self->numberOfAddressesToAnnounce >= self->maxAddressesToAnnounce){
    return;
  }

  self->addresses[self->numberOfAddressesToAnnounce] = address;
  self->namesOfAddresses[self->numberOfAddressesToAnnounce] = (char*) name;
  self->typesOfAddresses[self->numberOfAddressesToAnnounce] = (char*) type;

  self->numberOfAddressesToAnnounce++;
  incrementPayloadField(self);

  if(self->addressesArePrepared == false){
    self->scDataFields += 1;
  }

  self->addressesArePrepared = true;
}

static void packChannel(MsgpackPackerHandle self){

  if(self->channelsArePrepared == false){
    return;
  }

  msgpack_pack_str(&self->pkPayload, strlen(KEYWORD_CL_DATA));
  msgpack_pack_str_body(&self->pkPayload, KEYWORD_CL_DATA, strlen(KEYWORD_CL_DATA));
  msgpack_pack_map(&self->pkPayload, self->numberOfChannelsToSend);

  for (size_t i = 0; i < self->numberOfChannelsToSend; ++i) {

    char id[10];
    sprintf(id, "%d", self->channelIds[i]);

    msgpack_pack_str(&self->pkPayload, strlen(id));
    msgpack_pack_str_body(&self->pkPayload, id, strlen(id));

    const size_t dataLength = self->floatStreams[i]->length(self->floatStreams[i]);
    float data[dataLength];

    self->floatStreams[i]->read(self->floatStreams[i], data, dataLength);

    msgpack_pack_array(&self->pkPayload, dataLength);

    for (int j = 0; j < dataLength; ++j) {
      msgpack_pack_float(&self->pkPayload, data[j]);
    }
  }

  self->channelsArePrepared = false;
}

static void packTimestamp(MsgpackPackerHandle self){

  if(self->timestampIsPrepared == false){
    return;
  }

  msgpack_pack_str(&self->pkPayload, strlen(KEYWORD_T_STMP));
  msgpack_pack_str_body(&self->pkPayload, KEYWORD_T_STMP, strlen(KEYWORD_T_STMP));

  const size_t dataLength = self->currentTimestamp->length(self->currentTimestamp);
  gemmi_uint data[dataLength];

  self->currentTimestamp->read(self->currentTimestamp, data, dataLength);
  msgpack_pack_array(&self->pkPayload, dataLength);
  for (int i = 0; i < dataLength; ++i) {
#if (ARCH_SIZE_32)
    msgpack_pack_uint32(&self->pkPayload, (gemmi_uint) data[i]);
#else
    msgpack_pack_uint64(&self->pkPayload, (gemmi_uint) data[i]);
#endif
  }

}

static void packTimestampIncrement(MsgpackPackerHandle self){

  if(self->timestampIncrementIsPrepared == false){
    return;
  }

  msgpack_pack_str(&self->pkPayload, strlen(KEYWORD_T_INC));
  msgpack_pack_str_body(&self->pkPayload, KEYWORD_T_INC, strlen(KEYWORD_T_INC));

#if (ARCH_SIZE_32)
  msgpack_pack_uint32(&self->pkPayload, self->timestampIncrement);
#else
  msgpack_pack_uint64(&self->pkPayload, self->timestampIncrement);
#endif
}

static void packTrigger(MsgpackPackerHandle self){

  if(self->triggerIsPrepared == false){
    return;
  }

  msgpack_pack_str(&self->pkPayload, strlen(KEYWORD_TGR));
  msgpack_pack_str_body(&self->pkPayload, KEYWORD_TGR, strlen(KEYWORD_TGR));
  msgpack_pack_map(&self->pkPayload, 3);

  msgpack_pack_str(&self->pkPayload, strlen(KEYWORD_TGR_FOUND));
  msgpack_pack_str_body(&self->pkPayload, KEYWORD_TGR_FOUND, strlen(KEYWORD_TGR_FOUND));

  if(self->isTriggered == true){
    msgpack_pack_true(&self->pkPayload);
  } else {
    msgpack_pack_false(&self->pkPayload);
  }


  msgpack_pack_str(&self->pkPayload, strlen(KEYWORD_TGR_CL_DATA_IND));
  msgpack_pack_str_body(&self->pkPayload, KEYWORD_TGR_CL_DATA_IND, strlen(KEYWORD_TGR_CL_DATA_IND));

#if (ARCH_SIZE_32)
  msgpack_pack_uint32(&self->pkPayload, self->triggerTimestamp);
#else
  msgpack_pack_uint64(&self->pkPayload, self->triggerTimestamp);
#endif

  msgpack_pack_str(&self->pkPayload, strlen(KEYWORD_TGR_CL_ID));
  msgpack_pack_str_body(&self->pkPayload, KEYWORD_TGR_CL_ID, strlen(KEYWORD_TGR_CL_ID));

  char id[10];
  sprintf(id, "%d", self->activeChannelId);

  msgpack_pack_str(&self->pkPayload, strlen(id));
  msgpack_pack_str_body(&self->pkPayload, id, strlen(id));
}


static void packFlowControl(MsgpackPackerHandle self){

  if(self->flowControlIsPrepared == false){
    return;
  }

  /* Flow control map */
  msgpack_pack_str(&self->pkPayload, strlen(KEYWORD_FLOW_CTRL));
  msgpack_pack_str_body(&self->pkPayload, KEYWORD_FLOW_CTRL, strlen(KEYWORD_FLOW_CTRL));
  msgpack_pack_str(&self->pkPayload, strlen(self->flowcontrol));
  msgpack_pack_str_body(&self->pkPayload, self->flowcontrol, strlen(self->flowcontrol));
}

static void packAddresses(MsgpackPackerHandle self){

  if(self->addressesArePrepared == false){
    return;
  }

  msgpack_pack_str(&self->pkPayload, strlen(KEYWORD_ANNOUNCE));
  msgpack_pack_str_body(&self->pkPayload, KEYWORD_ANNOUNCE, strlen(KEYWORD_ANNOUNCE));
  msgpack_pack_map(&self->pkPayload, self->numberOfAddressesToAnnounce + 1);

  for (size_t i = 0; i < self->numberOfAddressesToAnnounce; ++i) {

    msgpack_pack_str(&self->pkPayload, strlen(self->namesOfAddresses[i]));
    msgpack_pack_str_body(&self->pkPayload, self->namesOfAddresses[i], strlen(self->namesOfAddresses[i]));

    msgpack_pack_array(&self->pkPayload, 2);

#if (ARCH_SIZE_32)
    msgpack_pack_uint32(&self->pkPayload, self->addresses[i]);
#else
    msgpack_pack_uint64(&self->pkPayload, self->addresses[i]);
#endif

    msgpack_pack_str(&self->pkPayload, strlen(self->typesOfAddresses[i]));
    msgpack_pack_str_body(&self->pkPayload, self->typesOfAddresses[i], strlen(self->typesOfAddresses[i]));
  }

  msgpack_pack_str(&self->pkPayload, strlen(KEYWORD_NUMBER_OF_CHANNELS));
  msgpack_pack_str_body(&self->pkPayload, KEYWORD_NUMBER_OF_CHANNELS, strlen(KEYWORD_NUMBER_OF_CHANNELS));
  
  #if (ARCH_SIZE_32)
      msgpack_pack_uint32(&self->pkPayload, self->maxNumberOfChannels);
  #else
      msgpack_pack_uint64(&self->pkPayload, self->maxNumberOfChannels);
  #endif

  self->addressesArePrepared = false;
}

static void constructBase(MsgpackPackerHandle self){

  msgpack_pack_map(&self->pk, 2);

  /* Construct transport map */
  msgpack_pack_str(&self->pk, strlen(KEYWORD_TRANSPORT));
  msgpack_pack_str_body(&self->pk, KEYWORD_TRANSPORT, strlen(KEYWORD_TRANSPORT));


  if(self->validator->checkPresentInProtocol(self->validator) == true){
    const size_t lengthCheck = self->validator->getCheckLength(self->validator);
    uint8_t check[lengthCheck];

    self->validator->createCheck(self->validator,check, (const uint8_t*) self->sbufPayload.data, self->sbufPayload.size);

    msgpack_pack_bin(&self->pk, lengthCheck);
    msgpack_pack_bin_body(&self->pk, check, lengthCheck);
  } else {
    msgpack_pack_nil(&self->pk);
  }

  /* Construct payload map */
  msgpack_pack_str(&self->pk, strlen(KEYWORD_PAYLOAD));
  msgpack_pack_str_body(&self->pk, KEYWORD_PAYLOAD, strlen(KEYWORD_PAYLOAD));
}

static void constructPayloadMap(MsgpackPackerHandle self){

  /* Construct sc_data and flow_ctrl */
  msgpack_pack_map(&self->pkPayload, self->payloadFields);
}

static void constructScDataMap(MsgpackPackerHandle self){

  /* sc_data map */
  if(self->scDataFields <= 0){
    return;
  }

  msgpack_pack_str(&self->pkPayload, strlen(KEYWORD_SC_DATA));
  msgpack_pack_str_body(&self->pkPayload, KEYWORD_SC_DATA, strlen(KEYWORD_SC_DATA));
  msgpack_pack_map(&self->pkPayload, self->scDataFields);
}

static void packData(IPackerHandle iPacker){
  MsgpackPackerHandle self = (MsgpackPackerHandle) iPacker->implementer;

  msgpack_sbuffer_clear(&self->sbuf);
  msgpack_sbuffer_clear(&self->sbufPayload);

  constructPayloadMap(self);

  constructScDataMap(self);

  packChannel(self);

  packTimestamp(self);

  packTimestampIncrement(self);

  packTrigger(self);

  packAddresses(self);

  packFlowControl(self);

  constructBase(self);

  self->byteStream->flush(self->byteStream);
  self->byteStream->write(self->byteStream, (const uint8_t*) self->sbuf.data, self->sbuf.size);
  self->byteStream->write(self->byteStream, (const uint8_t*) self->sbufPayload.data, self->sbufPayload.size);

  iPacker->reset(iPacker);
}

static void reset(IPackerHandle iPacker){
  MsgpackPackerHandle self = (MsgpackPackerHandle) iPacker->implementer;

  self->payloadFields = 0;
  self->scDataFields = 0;
  self->triggerIsPrepared = false;
  self->timestampIncrementIsPrepared = false;
  self->channelsArePrepared = false;
  self->timestampIsPrepared = false;
  self->flowControlIsPrepared = false;
  self->numberOfChannelsToSend = 0;
  self->addressesArePrepared = false;
  self->numberOfAddressesToAnnounce = 0;
}

static IByteStreamHandle getByteStream(IPackerHandle iPacker){
  MsgpackPackerHandle self = (MsgpackPackerHandle) iPacker->implementer;
  return self->byteStream;
}
/******************************************************************************
 Public functions
******************************************************************************/
MsgpackPackerHandle MsgpackPacker_create(const size_t msgLength, const size_t maxNumberOfChannels,
                                         const size_t maxAddressesToAnnounce,
                                         IByteStreamHandle byteStream,
                                         IComValidatorHandle validator){

  MsgpackPackerHandle self = (MsgpackPackerHandle) malloc(sizeof(MsgpackPackerPrivateData));

  self->floatStreams = malloc(sizeof(IFloatStreamHandle) * maxNumberOfChannels);
  self->channelIds = malloc(sizeof(gemmi_uint) * maxNumberOfChannels);
  self->byteStream = byteStream;
  self->validator = validator;
  self->numberOfChannelsToSend = 0;
  self->maxNumberOfChannels = maxNumberOfChannels;
  self->maxAddressesToAnnounce = maxAddressesToAnnounce;
  self->addresses = malloc(sizeof(gemmi_uint) * maxAddressesToAnnounce);
  self->namesOfAddresses = malloc(sizeof(char*) * maxAddressesToAnnounce);
  self->typesOfAddresses = malloc(sizeof(char*) * maxAddressesToAnnounce);

  self->scDataFields = 0;
  self->payloadFields = 0;

  self->iPacker.implementer = self;
  self->iPacker.pack = &packData;
  self->iPacker.reset = &reset;
  self->iPacker.prepareChannel = prepareChannel;
  self->iPacker.prepareFlowControl = &prepareFlowControl;
  self->iPacker.prepareTimeIncrement = &prepareTimeIncrement;
  self->iPacker.prepareTimestamp = &prepareTimestamp;
  self->iPacker.prepareTrigger = &prepareTrigger;
  self->iPacker.getByteStream = &getByteStream;
  self->iPacker.prepareAddressAnnouncement = &prepareAddressAnnouncement;

  msgpack_sbuffer_init(&self->sbuf);
  msgpack_packer_init(&self->pk, &self->sbuf, msgpack_sbuffer_write);

  msgpack_sbuffer_init(&self->sbufPayload);
  msgpack_packer_init(&self->pkPayload, &self->sbufPayload, msgpack_sbuffer_write);

  self->iPacker.reset(&self->iPacker);

  return self;
}

void MsgpackPacker_destroy(MsgpackPackerHandle self){

  free(self->addresses);
  self->addresses = NULL;
  free(self->namesOfAddresses);
  self->namesOfAddresses = NULL;
  free(self->typesOfAddresses);
  self->typesOfAddresses = NULL;
  free(self->floatStreams);
  self->floatStreams = NULL;
  free(self->channelIds);
  self->channelIds = NULL;
  free(self);
  msgpack_sbuffer_destroy(&self->sbuf);
  self = NULL;
}

IPackerHandle MsgpackPacker_getIPacker(MsgpackPackerHandle self){
  return &self->iPacker;
}
