/*!*****************************************************************************
 * @file         Scope.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
*******************************************************************************/

#include <Scope/Core/Scope.h>
#include <Scope/GeneralPurpose/BufferedIntStream.h>
#include <Scope/Core/Timestamper.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __ScopePrivateData{
    IRunnable runnable;
    IScope scope;

    /* Scope data */
    size_t amountOfChannels;
    ChannelHandle* channels;
    TriggerHandle trigger;
    size_t channelSize;
    AddressStorageHandle addressStorage;

    TimestamperHandle timestamper;

    /* Flags */
    bool scopeIsReadyToSend;
    bool dataIsReadyToSend;

} ScopePrivateData;

bool allChannelsAreStopped(ScopeHandle self);

/******************************************************************************
 Private functions
******************************************************************************/
static void scopePoll(IScopeHandle scope){
    ScopeHandle self = (ScopeHandle) scope->handle;

    Scope_poll(self);
}

static void scopeClear(IScopeHandle scope){
    ScopeHandle self = (ScopeHandle) scope->handle;

    Scope_clear(self);
}

static void scopeSetTimeIncrement(IScopeHandle scope, uint32_t timeIncrement){
    ScopeHandle self = (ScopeHandle) scope->handle;

    Scope_configureTimestampIncrement(self, timeIncrement);
}

static uint32_t getTimeIncrement(IScopeHandle scope){
    ScopeHandle self = (ScopeHandle) scope->handle;

    return Timerstamper_getTimeIncrement(self->timestamper);
}

static size_t getAmountOfChannels(IScopeHandle scope){
    ScopeHandle self = (ScopeHandle) scope->handle;

    return self->amountOfChannels;
}

static IIntStreamHandle getTimestamp(IScopeHandle scope){
    ScopeHandle self = (ScopeHandle) scope->handle;

    return Timestamper_getStream(self->timestamper);
}

static void configureTrigger(IScopeHandle scope, TriggerConfiguration conf){
    ScopeHandle self = (ScopeHandle) scope->handle;

    Trigger_configure(self->trigger, conf);
}

static void setChannelRunning(IScopeHandle scope, uint32_t idOfChangedChannel){
    ScopeHandle self = (ScopeHandle) scope->handle;

    Scope_setChannelRunning(self, idOfChangedChannel);
}

static void setChannelStopped(IScopeHandle scope, uint32_t idOfChangedChannel){
    ScopeHandle self = (ScopeHandle) scope->handle;

    Scope_setChannelStopped(self, idOfChangedChannel);
}

static void configureChannelAddress(IScopeHandle scope, void* address,
                                    uint32_t idOfChangedChannel, DATA_TYPES typeOfAddress){
    ScopeHandle self = (ScopeHandle) scope->handle;

    Scope_configureChannel(self, idOfChangedChannel, address, typeOfAddress);
}

void announce(IScopeHandle scope){
    ScopeHandle self = (ScopeHandle) scope->handle;

    Scope_announce(self);
}

void transmit(IScopeHandle scope){
    ScopeHandle self = (ScopeHandle) scope->handle;

    Scope_transmit(self);
}

bool scopeIsReadyToSend(IScopeHandle scope){
    ScopeHandle self = (ScopeHandle) scope->handle;

    return self->scopeIsReadyToSend;
}

bool dataIsReadyToSend(IScopeHandle scope){
    ScopeHandle self = (ScopeHandle) scope->handle;

    return self->dataIsReadyToSend;
}

bool announcementIsReadyToSend(IScopeHandle scope){
    ScopeHandle self = (ScopeHandle) scope->handle;

    if(self->addressStorage == NULL){
        return false;
    }

    return AddressStorage_hasToBeSent(self->addressStorage);
}

void dataIsTransmitted(IScopeHandle scope){
    ScopeHandle self = (ScopeHandle) scope->handle;

    self->dataIsReadyToSend = false;
    self->scopeIsReadyToSend = false;
    Trigger_dataIsTransmitted(self->trigger);

    if(self->addressStorage ==  NULL){
        return;
    }

    AddressStorage_hasBeenSent(self->addressStorage);
}


static void run(IRunnableHandle runnable){
    ScopeHandle self = (ScopeHandle) runnable->handle;

    Scope_poll(self);
}

TriggeredValues getTriggerData(IScopeHandle scope){
    ScopeHandle self = (ScopeHandle) scope->handle;

    TriggeredValues values;

    values.isTriggered = Trigger_isTriggered(self->trigger);
    values.triggerTimestamp = Trigger_getTriggerIndex(self->trigger);
    values.channelId = Trigger_getChannelId(self->trigger);

    return values;
}

bool channelHasToBePacked(IScopeHandle scope, uint32_t channelId){
    ScopeHandle self = (ScopeHandle) scope->handle;

    if(channelId >= self->amountOfChannels){
        return false;
    }

    return Channel_getAmountOfUsedData(self->channels[channelId]) > 0;
}

int readChannelData(IScopeHandle scope, float data[], size_t size, uint32_t channelId){
    ScopeHandle self = (ScopeHandle) scope->handle;

    if(channelId >= self->amountOfChannels){
        return false;
    }

    return Channel_read(self->channels[channelId], data, size);
}

size_t getAmountOfUsedChannelData(IScopeHandle scope, uint32_t channelId){
    ScopeHandle self = (ScopeHandle) scope->handle;

    if(channelId >= self->amountOfChannels){
        return false;
    }

    return Channel_getAmountOfUsedData(self->channels[channelId]);
}

AddressDefinition* getAnnounceAddressToTransmit(IScopeHandle scope, uint32_t addressId){
    ScopeHandle self = (ScopeHandle) scope->handle;

    if(self->addressStorage == NULL){
        return NULL;
    }

    if(addressId >= AddressStorage_getMaxAmountOfAddresses(self->addressStorage)){
        return NULL;
    }

    return AddressStorage_getAddressToTransmit(self->addressStorage, addressId);
}

size_t getMaxAmmountOfAnnounceAddresses(IScopeHandle scope){
    ScopeHandle self = (ScopeHandle) scope->handle;

    if(self->addressStorage == NULL){
        return 0;
    }

    return AddressStorage_getMaxAmountOfAddresses(self->addressStorage);
}

size_t getMaxSizeOfChannel(IScopeHandle scope){
    ScopeHandle self = (ScopeHandle) scope->handle;

    return self->channelSize;
}

bool allChannelsAreStopped(ScopeHandle self){
    bool channelIsRunning = false;

    for(int i = 0; i < self->amountOfChannels; ++i){
       channelIsRunning = Channel_isRunning(self->channels[i]);
       if(channelIsRunning == true){
           break;
       }
    }
    return !channelIsRunning;
}

/******************************************************************************
 Public functions
******************************************************************************/
ScopeHandle Scope_create(size_t channelSize,
                         size_t amountOfChannels,
                         AddressStorageHandle addressStorage,
                         uint32_t* referenceTimestamp){

    ScopeHandle self = malloc(sizeof(ScopePrivateData));

    self->scopeIsReadyToSend = false;
    self->dataIsReadyToSend = false;
    self->channelSize = channelSize;
    self->addressStorage = addressStorage;

    self->scope.handle = self;
    self->scope.poll = &scopePoll;
    self->scope.setTimeIncrement = &scopeSetTimeIncrement;
    self->scope.getTimeIncrement = &getTimeIncrement;
    self->scope.getTimestamp = &getTimestamp;
    self->scope.clear = &scopeClear;
    self->scope.getAmountOfChannels = &getAmountOfChannels;
    self->scope.setChannelStopped = &setChannelStopped;
    self->scope.setChannelRunning = &setChannelRunning;
    self->scope.configureChannelAddress = &configureChannelAddress;
    self->scope.configureTrigger = &configureTrigger;
    self->scope.announcementIsReadyToSend = &announcementIsReadyToSend;
    self->scope.scopeIsReadyToSend = &scopeIsReadyToSend;
    self->scope.dataIsReadyToSend = &dataIsReadyToSend;
    self->scope.dataIsTransmitted = &dataIsTransmitted;
    self->scope.getTriggerData = &getTriggerData;
    self->scope.channelHasToBePacked = &channelHasToBePacked;
    self->scope.readChannelData = &readChannelData;
    self->scope.getAmountOfUsedChannelData = &getAmountOfUsedChannelData;
    self->scope.getAnnounceAddressToTransmit = &getAnnounceAddressToTransmit;
    self->scope.getMaxAmmountOfAnnounceAddresses = &getMaxAmmountOfAnnounceAddresses;
    self->scope.getMaxSizeOfChannel = &getMaxSizeOfChannel;
    self->scope.announce = &announce;
    self->scope.transmit = &transmit;

    self->runnable.handle = self;
    self->runnable.run = &run;

    self->channels = malloc(sizeof(ChannelHandle) * amountOfChannels);
    self->amountOfChannels = amountOfChannels;

    for(size_t i = 0; i < amountOfChannels; i++){
        self->channels[i] = Channel_create(channelSize);
    }

    self->timestamper = Timestamper_create(channelSize, referenceTimestamp);
    self->trigger = Trigger_create(self->channels, self->amountOfChannels, self->channelSize, self->timestamper);


    return self;
}

void Scope_destroy(ScopeHandle self){

    for(size_t i = 0; i < self->amountOfChannels; ++i){
        Channel_destroy(self->channels[i]);
        free(self->channels[i]);
        self->channels[i] = NULL;
    }

    Timestamper_destroy(self->timestamper);
    Trigger_destroy(self->trigger);

    free(self);
    self = NULL;
}

void Scope_transmit(ScopeHandle self){

    self->scopeIsReadyToSend = true;
    self->dataIsReadyToSend = true;
}

void Scope_announce(ScopeHandle self){

    if(self->addressStorage == NULL){
        return;
    }

    self->scopeIsReadyToSend = true;
    AddressStorage_announce(self->addressStorage);
}

void Scope_poll(ScopeHandle self){


    /* Check if the scope is ready to poll again, according to the set timeIncrement */
    if(Timestamper_updateElapsedTime(self->timestamper) == false){
        return;
    }

    /* Update timestamp */
    Timestamper_stamp(self->timestamper);

    for(size_t i = 0; i < self->amountOfChannels; i++){
        Channel_poll(self->channels[i]);
    }

    bool readyToSend = Trigger_run(self->trigger);
    if(readyToSend == true){
        Scope_transmit(self);
    }
}

void Scope_configureChannel(ScopeHandle self, const size_t channelId, void* pollAddress, DATA_TYPES type){

    if(channelId >= self->amountOfChannels){
        return;
    }

    Channel_setPollAddress(self->channels[channelId], pollAddress, type);
}

void Scope_configureTrigger(ScopeHandle self, const float level, int edge, TRIGGER_MODE mode, uint32_t channelId){

    if(channelId >= self->amountOfChannels){
        return;
    }

    TriggerConfiguration triggerConf = {
            .level = level,
            .edge = edge,
            .mode = mode,
            .channelId = channelId,
    };

    Trigger_configure(self->trigger, triggerConf);
}

void Scope_configureTimestampIncrement(ScopeHandle self, uint32_t timstampIncrement){
    Timestamper_configureTimestampIncrement(self->timestamper, timstampIncrement);
}

void Scope_setChannelRunning(ScopeHandle self, uint32_t channelId){

    if(channelId >= self->amountOfChannels){
        return;
    }

    Channel_setStateRunning(self->channels[channelId]);

    /* Check if the timestamper has to be stopped too */
    if(allChannelsAreStopped(self) == false){
        Timestamper_setStateRunning(self->timestamper);
        Trigger_activate(self->trigger);
    }
}

void Scope_setChannelStopped(ScopeHandle self, uint32_t channelId){

    if(channelId >= self->amountOfChannels){
        return;
    }

    Channel_setStateStopped(self->channels[channelId]);

    /* Check if the timestamper has to be stopped too */
    if(allChannelsAreStopped(self) == true){
        Timestamper_setStateStopped(self->timestamper);
        Trigger_deactivate(self->trigger);
    }
}

void Scope_clear(ScopeHandle self){
    for(size_t i = 0; i < self->amountOfChannels; i++){
        Channel_clear(self->channels[i]);
    }
    Timestamper_clear(self->timestamper);
    Trigger_deactivate(self->trigger);
    Trigger_activate(self->trigger);
}

void Scope_addAnnounceAddresses(ScopeHandle self, const char* name, const void* address,
                                const DATA_TYPES type,
                                const uint32_t addressId){

    if(self->addressStorage == NULL){
        return;
    }

    AddressStorage_addAnnounceAddress(self->addressStorage, name, address, type, addressId);
}

IRunnableHandle Scope_getIRunnable(ScopeHandle self){
    return &self->runnable;
}

IScopeHandle Scope_getIScope(ScopeHandle self){
    return &self->scope;
}
