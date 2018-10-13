/*!****************************************************************************************************************************************
 * @file         MsgpackPacker.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <MsgpackParser/MsgpackPacker.h>
#include <msgpack/object.h>
#include <msgpack.h>
#include <string.h>
#include <stdio.h>
#include <GeneralPurpose/Memory.h>


static const char* KEYWORD_PAYLOAD = "payload";
static const char* KEYWORD_TRANSPORT = "transport";
static const char* KEYWORD_SC_DATA = "sc_data";
static const char* KEYWORD_PLACEHOLDER = "...";
static const char* KEYWORD_FLOW_CTRL = "flow_ctrl";
static const char* KEYWORD_CL_DATA = "cl_data";
static const char* KEYWORD_T_STMP = "t_stmp";
static const char* KEYWORD_T_INC = "t_inc";
static const char* KEYWORD_TGR = "tgr";
static const char* KEYWORD_TGR_FOUND = "found";
static const char* KEYWORD_TGR_CL_DATA_IND = "cl_data_ind";
static const char* KEYWORD_TGR_CL_ID = "cl_id";

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

  uint32_t scDataFields;
  uint32_t payloadFields;

  IByteStreamHandle byteStream;


  /* Channel preparation data */
  bool channelsArePrepared;
  size_t numberOfChannelsToSend;
  uint32_t* channelIds;

  /* Trigger preparation data */
  bool triggerIsPrepared;
  bool isTriggered;
  uint32_t triggerTimestamp;
  uint32_t activeChannelId;
  IFloatStreamHandle* floatStreams;

  /* Timestamp preparation data */
  bool timestampIsPrepared;
  uint32_t currentTimestamp;

  /* Timestamp increment preparation data */
  bool timestampIncrementIsPrepared;
  uint32_t timestampIncrement;

  /* Flow Control preparation data */
  bool flowControlIsPrepared;
  char flowcontrol[4];

  /* transport data preparation data */
  bool transportDataIsPrepared;

} MsgpackPackerPrivateData ;

/******************************************************************************
 Private functions
******************************************************************************/
static void printObj(IPackerHandle iPacker){
  MsgpackPackerHandle self = (MsgpackPackerHandle) iPacker->implementer;

  msgpack_zone mempool;
  msgpack_object deserialized;


  /* deserialize the buffer into msgpack_object instance. */
  /* deserialized object is valid during the msgpack_zone instance alive. */
  msgpack_zone_init(&mempool, 2048);

  msgpack_unpack(self->sbuf.data, self->sbuf.size, NULL, &mempool, &deserialized);

  /* print the deserialized object. */
  msgpack_object_print(stdout, deserialized);
}

static void incrementPayloadField(MsgpackPackerHandle self){

  if((self->channelsArePrepared == false) && (self->triggerIsPrepared == false) &&
          (self->timestampIsPrepared == false) && (self->timestampIncrementIsPrepared == false)){
    self->payloadFields += 1;
  }
}

static void prepareChannel(IPackerHandle iPacker, IFloatStreamHandle stream, const uint32_t channelId){
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

static void prepareTimeIncrement(IPackerHandle iPacker, const uint32_t timeIncrement){
  MsgpackPackerHandle self = (MsgpackPackerHandle) iPacker->implementer;

  if(self->timestampIncrementIsPrepared == true){
    return;
  }

  self->timestampIncrement = timeIncrement;
  incrementPayloadField(self);
  self->scDataFields += 1;
  self->timestampIncrementIsPrepared = true;
}

static void prepareTimestamp(IPackerHandle iPacker, const uint32_t timestamp){
  MsgpackPackerHandle self = (MsgpackPackerHandle) iPacker->implementer;

  if(self->timestampIsPrepared == true){
    return;
  }

  self->currentTimestamp = timestamp;
  incrementPayloadField(self);
  self->scDataFields += 1;
  self->timestampIsPrepared = true;
}

static void prepareTrigger(IPackerHandle iPacker, const bool isTriggered, const uint32_t channelId, const uint32_t timestamp){
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
  self->payloadFields += 1;
}

static void prepareTransportData(IPackerHandle iPacker){
  MsgpackPackerHandle self = (MsgpackPackerHandle) iPacker->implementer;

  self->transportDataIsPrepared = false;
}

static void pack(IPackerHandle iPacker){
  MsgpackPackerHandle self = (MsgpackPackerHandle) iPacker->implementer;

  self->flowControlIsPrepared = false;
}

static void packChannel(MsgpackPackerHandle self){

  if(self->channelsArePrepared == false){
    return;
  }

  msgpack_pack_str(&self->pk, strlen(KEYWORD_CL_DATA));
  msgpack_pack_str_body(&self->pk, KEYWORD_CL_DATA, strlen(KEYWORD_CL_DATA));
  msgpack_pack_map(&self->pk, self->numberOfChannelsToSend);

  for (size_t i = 0; i < self->numberOfChannelsToSend; ++i) {

    char id[10];
    sprintf(id, "%d", self->channelIds[i] + 1);

    msgpack_pack_str(&self->pk, strlen(id));
    msgpack_pack_str_body(&self->pk, id, strlen(id));

    const size_t dataLength = self->floatStreams[i]->getSize(self->floatStreams[i]);
    float data[dataLength];

    self->floatStreams[i]->open(self->floatStreams[i], data);
    self->floatStreams[i]->getStream(self->floatStreams[i]);

    msgpack_pack_array(&self->pk, dataLength);

    for (int j = 0; j < dataLength; ++j) {
      msgpack_pack_float(&self->pk, data[j]);
    }
  }

  self->channelsArePrepared = false;
}

static void packTimestamp(MsgpackPackerHandle self){

  if(self->timestampIsPrepared == false){
    return;
  }

  msgpack_pack_str(&self->pk, strlen(KEYWORD_T_STMP));
  msgpack_pack_str_body(&self->pk, KEYWORD_T_STMP, strlen(KEYWORD_T_STMP));
  msgpack_pack_int32(&self->pk, self->currentTimestamp);
}

static void packTimestampIncrement(MsgpackPackerHandle self){

  if(self->timestampIncrementIsPrepared == false){
    return;
  }

  msgpack_pack_str(&self->pk, strlen(KEYWORD_T_INC));
  msgpack_pack_str_body(&self->pk, KEYWORD_T_INC, strlen(KEYWORD_T_INC));
  msgpack_pack_int32(&self->pk, self->timestampIncrement);
}

static void packTrigger(MsgpackPackerHandle self){

  if(self->triggerIsPrepared == false){
    return;
  }

  msgpack_pack_str(&self->pk, strlen(KEYWORD_TGR));
  msgpack_pack_str_body(&self->pk, KEYWORD_TGR, strlen(KEYWORD_TGR));
  msgpack_pack_map(&self->pk, 3);

  msgpack_pack_str(&self->pk, strlen(KEYWORD_TGR_FOUND));
  msgpack_pack_str_body(&self->pk, KEYWORD_TGR_FOUND, strlen(KEYWORD_TGR_FOUND));

  if(self->isTriggered == true){
    msgpack_pack_true(&self->pk);
  } else {
    msgpack_pack_false(&self->pk);
  }


  msgpack_pack_str(&self->pk, strlen(KEYWORD_TGR_CL_DATA_IND));
  msgpack_pack_str_body(&self->pk, KEYWORD_TGR_CL_DATA_IND, strlen(KEYWORD_TGR_CL_DATA_IND));
  msgpack_pack_int32(&self->pk, self->triggerTimestamp);

  msgpack_pack_str(&self->pk, strlen(KEYWORD_TGR_CL_ID));
  msgpack_pack_str_body(&self->pk, KEYWORD_TGR_CL_ID, strlen(KEYWORD_TGR_CL_ID));

  char id[10];
  sprintf(id, "%d", self->activeChannelId + 1);

  msgpack_pack_str(&self->pk, strlen(id));
  msgpack_pack_str_body(&self->pk, id, strlen(id));
}


static void packFlowControl(MsgpackPackerHandle self){

  if(self->flowControlIsPrepared == false){
    return;
  }

  /* Flow control map */
  msgpack_pack_str(&self->pk, strlen(KEYWORD_FLOW_CTRL));
  msgpack_pack_str_body(&self->pk, KEYWORD_FLOW_CTRL, strlen(KEYWORD_FLOW_CTRL));
  msgpack_pack_str(&self->pk, strlen(self->flowcontrol));
  msgpack_pack_str_body(&self->pk, self->flowcontrol, strlen(self->flowcontrol));
}

static void constructBase(MsgpackPackerHandle self){

  msgpack_pack_map(&self->pk, 2);

  /* Construct transport map */
  msgpack_pack_str(&self->pk, strlen(KEYWORD_TRANSPORT));
  msgpack_pack_str_body(&self->pk, KEYWORD_TRANSPORT, strlen(KEYWORD_TRANSPORT));
  msgpack_pack_map(&self->pk, 1);
  msgpack_pack_str(&self->pk, strlen(KEYWORD_PLACEHOLDER));
  msgpack_pack_str_body(&self->pk, KEYWORD_PLACEHOLDER, strlen(KEYWORD_PLACEHOLDER));
  msgpack_pack_str(&self->pk, strlen(KEYWORD_PLACEHOLDER));
  msgpack_pack_str_body(&self->pk, KEYWORD_PLACEHOLDER, strlen(KEYWORD_PLACEHOLDER));
}

static void constructPayloadMap(MsgpackPackerHandle self){

  /* Construct payload map */
  msgpack_pack_str(&self->pk, strlen(KEYWORD_PAYLOAD));
  msgpack_pack_str_body(&self->pk, KEYWORD_PAYLOAD, strlen(KEYWORD_PAYLOAD));

  /* Construct sc_data and flow_ctrl */
  msgpack_pack_map(&self->pk, self->payloadFields);
}

static void constructScDataMap(MsgpackPackerHandle self){

  /* sc_data map */
  msgpack_pack_str(&self->pk, strlen(KEYWORD_SC_DATA));
  msgpack_pack_str_body(&self->pk, KEYWORD_SC_DATA, strlen(KEYWORD_SC_DATA));
  msgpack_pack_map(&self->pk, self->scDataFields);
}

static void packData(IPackerHandle iPacker){
  MsgpackPackerHandle self = (MsgpackPackerHandle) iPacker->implementer;

  msgpack_sbuffer_clear(&self->sbuf);

  constructBase(self);

  constructPayloadMap(self);

  constructScDataMap(self);

  packChannel(self);

  packTimestamp(self);

  packTimestampIncrement(self);

  packTrigger(self);

  packFlowControl(self);

  self->byteStream->flush(self->byteStream);
  self->byteStream->write(self->byteStream, (const uint8_t*) self->sbuf.data, self->sbuf.size);

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
}

static IByteStreamHandle getByteStream(IPackerHandle iPacker){
  MsgpackPackerHandle self = (MsgpackPackerHandle) iPacker->implementer;
  return self->byteStream;
}
/******************************************************************************
 Public functions
******************************************************************************/
MsgpackPackerHandle MsgpackPacker_create(const size_t msgLength, const size_t maxNumberOfChannels,
                                          IByteStreamHandle byteStream){

  MsgpackPackerHandle self = (MsgpackPackerHandle) malloc(sizeof(MsgpackPackerPrivateData));

  self->floatStreams = malloc(sizeof(IFloatStreamHandle) * maxNumberOfChannels);
  self->channelIds = malloc(sizeof(uint32_t) * maxNumberOfChannels);
  self->byteStream = byteStream;
  self->numberOfChannelsToSend = 0;
  self->maxNumberOfChannels = maxNumberOfChannels;

  self->scDataFields = 0;
  self->payloadFields = 0;

  self->iPacker.implementer = self;
  self->iPacker.pack = &packData;
  self->iPacker.reset = &reset;
  self->iPacker.prepareChannel = prepareChannel;
  self->iPacker.prepareFlowControl = &prepareFlowControl;
  self->iPacker.prepareTimeIncrement = &prepareTimeIncrement;
  self->iPacker.prepareTimestamp = &prepareTimestamp;
  self->iPacker.prepareTransportData = &prepareTransportData;
  self->iPacker.prepareTrigger = &prepareTrigger;
  self->iPacker.print = &printObj;

  msgpack_sbuffer_init(&self->sbuf);
  msgpack_packer_init(&self->pk, &self->sbuf, msgpack_sbuffer_write);

  self->iPacker.reset(&self->iPacker);

  return self;
}

void MsgpackPacker_destroy(MsgpackPackerHandle self){
  free(self);
  self = NULL;
}

IPackerHandle MsgpackPacker_getIPacker(MsgpackPackerHandle self){
  return &self->iPacker;
}