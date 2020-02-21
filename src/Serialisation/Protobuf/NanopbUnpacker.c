/*!****************************************************************************************************************************************
 * @file         NanopbUnpacker.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <se-lib-c/stream/IByteStream.h>

#include "Scope/GeneralPurpose/DataTypes.h"
#include "Scope/Serialisation/Protobuf/NanopbUnpacker.h"
#include "Scope/Control/IUnpacker.h"
#include "Scope/Core/ScopeTypes.h"
#include "Scope/Serialisation/Protobuf/se-scope.pb.h"

#include <assert.h>
#include <pb.h>
#include <stdlib.h>
#include <pb_decode.h>

typedef struct __CfRunning {
    uint32_t amount;
    CfRunningDef* channels;
} CfRunning;

typedef struct __CfAddress {
    uint32_t amount;
    CfAddressDef* channels;
} CfAddress;

/******************************************************************************
 Define private data
******************************************************************************/
typedef struct __NanopbUnpackerPrivateData{

    IUnpacker unpacker;
    pb_istream_t wrapped;
    size_t maxNumberOfChannels;
    IByteStreamHandle input;

    uint32_t evPollTimestamp;
    CfRunning running;
    CfAddress address;
    CfTriggerDef triggerConfig;

    uint32_t tInc;

} NanopbUnpackerPrivateData;

static bool unpack(IUnpackerHandle unpacker, MessageType type);

static uint32_t evPoll_getTimestamp(IUnpackerHandle unpacker);

static size_t cfRunning_getAmount(IUnpackerHandle unpacker);

static CfRunningDef cfRunning_getChannel(IUnpackerHandle unpacker, uint32_t index);

static CfTriggerDef cfTrigger_getTriggerConfig(IUnpackerHandle unpacker);

static uint32_t cfTInc_getInc(IUnpackerHandle unpacker);

static uint32_t cfAddress_getAmount(IUnpackerHandle unpacker);

static CfAddressDef cfAddress_getChannel(IUnpackerHandle unpacker, uint32_t index);

static bool readBuffer(pb_istream_t* stream, pb_byte_t* buf, size_t count);

static bool addressConfigCallback(pb_istream_t *stream, const pb_field_t *field, void **arg){
    (void)(field);
    PB_AddressConfig config;
    bool status = pb_decode(stream, PB_AddressConfig_fields, &config);

    if(status == false){
        return false;
    }

    CfAddress* address =  (CfAddress*) (*arg);
    address->channels[address->amount].id = config.cl_id;
    address->channels[address->amount].type = (DATA_TYPES) config.type;
    address->channels[address->amount].address = config.address;
    address->amount += 1;

    return true;
}

static bool runningConfigCallback(pb_istream_t *stream, const pb_field_t *field, void **arg){
    (void)(field);
    PB_RunningConfig config;
    bool status = pb_decode(stream, PB_RunningConfig_fields, &config);

    if(status == false){
        return false;
    }

    CfRunning* running =  (CfRunning*) (*arg);
    running->channels[running->amount].id = config.cl_id;
    running->channels[running->amount].newState = config.new_state;
    running->amount += 1;

    return true;
}

/******************************************************************************
 Private functions
******************************************************************************/
static bool unpack(IUnpackerHandle unpacker, MessageType type){
    NanopbUnpackerHandle self = (NanopbUnpackerHandle) unpacker->handle;
    self->wrapped.bytes_left = self->input->length(self->input);

    if(type == EV_POLL){
        PB_EV_Poll poll = PB_EV_Poll_init_zero;
        bool status = pb_decode(&self->wrapped, PB_EV_Poll_fields, &poll);
        if(!status){
            return false;
        }
        self->evPollTimestamp = poll.timestamp;
    }else if(type == CF_RUNNING){
        PB_CF_Running running = PB_CF_Running_init_zero;
        running.running.arg = &self->running;
        running.running.funcs.decode = &runningConfigCallback;
        self->running.amount = 0;
        bool status = pb_decode(&self->wrapped, PB_CF_Running_fields, &running);
        if(!status){
            return false;
        }
    }else if(type == CF_TRIGGER){
        PB_CF_Trigger trigger = PB_CF_Trigger_init_zero;
        bool status = pb_decode(&self->wrapped, PB_CF_Trigger_fields, &trigger);
        if(!status){
            return false;
        }

        self->triggerConfig.channelId = trigger.cl_id;
        self->triggerConfig.mode = (TRIGGER_MODE) trigger.mode;
        self->triggerConfig.edge = trigger.edge;
        self->triggerConfig.level = trigger.level;

    }else if(type == CF_T_INC){
        PB_CF_TInc tInc = PB_CF_TInc_init_zero;
        bool status = pb_decode(&self->wrapped, PB_CF_TInc_fields, &tInc);
        if(!status){
            return false;
        }
        self->tInc = tInc.t_inc;
    }else if(type == CF_ADDR){
        PB_CF_Address address = PB_CF_Address_init_zero;
        address.addresses.arg = &self->address;
        address.addresses.funcs.decode = &addressConfigCallback;
        self->address.amount = 0;
        bool status = pb_decode(&self->wrapped, PB_CF_Address_fields, &address);
        if(!status){
            return false;
        }
    }
    return true;
}

static uint32_t evPoll_getTimestamp(IUnpackerHandle unpacker){
    NanopbUnpackerHandle self = (NanopbUnpackerHandle) unpacker->handle;
    return self->evPollTimestamp;
}

static size_t cfRunning_getAmount(IUnpackerHandle unpacker){
    NanopbUnpackerHandle self = (NanopbUnpackerHandle) unpacker->handle;
    return self->running.amount;
}

static CfRunningDef cfRunning_getChannel(IUnpackerHandle unpacker, uint32_t index){
    NanopbUnpackerHandle self = (NanopbUnpackerHandle) unpacker->handle;
    if(index > self->running.amount){
        return self->running.channels[0];
    }
    return self->running.channels[index];
}

static CfTriggerDef cfTrigger_getTriggerConfig(IUnpackerHandle unpacker){
    NanopbUnpackerHandle self = (NanopbUnpackerHandle) unpacker->handle;
    return self->triggerConfig;
}

static uint32_t cfTInc_getInc(IUnpackerHandle unpacker){
    NanopbUnpackerHandle self = (NanopbUnpackerHandle) unpacker->handle;
    return self->tInc;
}

static uint32_t cfAddress_getAmount(IUnpackerHandle unpacker){
    NanopbUnpackerHandle self = (NanopbUnpackerHandle) unpacker->handle;
    return self->address.amount;
}

static CfAddressDef cfAddress_getChannel(IUnpackerHandle unpacker, uint32_t index){
    NanopbUnpackerHandle self = (NanopbUnpackerHandle) unpacker->handle;
    if(index > self->address.amount){
        return self->address.channels[0];
    }
    return self->address.channels[index];
}

static bool readBuffer(pb_istream_t* stream, pb_byte_t* buf, size_t count){

    IByteStreamHandle input = (IByteStreamHandle) stream->state;

    if(count > input->length(input)){
        return false;
    }

    input->read(input, (uint8_t*) buf, count);
    return true;
}

/******************************************************************************
 Public functions
******************************************************************************/
NanopbUnpackerHandle NanopbUnpacker_create(IByteStreamHandle input, size_t amountOfChannels){

    NanopbUnpackerHandle self = (NanopbUnpackerHandle) malloc(sizeof(NanopbUnpackerPrivateData));

    assert(self);

    self->input = input;

    self->wrapped.callback = &readBuffer;
    self->wrapped.bytes_left = 0;
    self->wrapped.state = self->input;
    self->maxNumberOfChannels = amountOfChannels;

    self->address.channels = (CfAddressDef*) malloc(sizeof(CfAddressDef) * amountOfChannels);
    self->running.channels = (CfRunningDef*) malloc(sizeof(CfRunningDef) * amountOfChannels);

    self->unpacker.handle = self;
    self->unpacker.cfAddress_getAmount = &cfAddress_getAmount;
    self->unpacker.cfAddress_getChannel = &cfAddress_getChannel;
    self->unpacker.cfRunning_getAmount = &cfRunning_getAmount;
    self->unpacker.cfRunning_getChannel = &cfRunning_getChannel;
    self->unpacker.cfTInc_getInc = &cfTInc_getInc;
    self->unpacker.cfTrigger_getTriggerConfig = &cfTrigger_getTriggerConfig;
    self->unpacker.evPoll_getTimestamp = &evPoll_getTimestamp;
    self->unpacker.unpack = &unpack;

    return self;
}

void NanopbUnpacker_destroy(NanopbUnpackerHandle self){
    free(self->running.channels);
    self->running.channels = NULL;
    free(self->address.channels);
    self->address.channels = NULL;

    free(self);
    self = NULL;
}

size_t NanopbUnpacker_calculateBufferSize(size_t maxNumberOfChannels){

    size_t possibleSizes = 5;

    size_t sizes[] = {PB_CF_Trigger_size, PB_CF_TInc_size, PB_EV_Poll_size, PB_AddressConfig_size * maxNumberOfChannels,
                      PB_RunningConfig_size * maxNumberOfChannels};

    size_t biggest = 0;

    for (size_t i = 0; i < possibleSizes; ++i) {
        if(sizes[i] > biggest){
            biggest = sizes[i];
        }
    }

	return biggest;
}

IUnpackerHandle NanopbUnpacker_getIUnpacker(NanopbUnpackerHandle self){
    return &self->unpacker;
}
