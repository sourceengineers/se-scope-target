/*!****************************************************************************************************************************************
* @file         CommandPack.c
*
* @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
*
* @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
*
*****************************************************************************************************************************************/

#include <Scope/Control/Command/CommandPack.h>
#include <Scope/Control/IPacker.h>
#include <Scope/Core/ScopeTypes.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandPackPrivateData{
    ICommand command;
    IScopeHandle scope;

    IPackerHandle packer;

    float** channelData; // Buffers to safe data from channels. This might not be the most ideal solution,
    // since it takes up as much space as the channels themselves
} CommandPackPrivateData;

/******************************************************************************
 Private functions
******************************************************************************/
static void run(ICommandHandle command){
    CommandPackHandle self = (CommandPackHandle) command->handle;

    if(self->scope->dataIsReadyToSend(self->scope)){

        for(uint32_t i = 0; i < self->scope->getAmountOfChannels(self->scope); ++i){
            if(self->scope->channelHasToBePacked(self->scope, i) == true){
                size_t length = self->scope->getAmountOfUsedChannelData(self->scope, i);
                self->scope->readChannelData(self->scope, self->channelData[i], length, i);
                self->packer->prepareChannel(self->packer, self->channelData[i], length, i);
            }
        }

        TriggeredValues triggeredValues = self->scope->getTriggerData(self->scope);
        self->packer->prepareTrigger(self->packer, triggeredValues.isTriggered, triggeredValues.channelId, \
                                     triggeredValues.triggerTimestamp);

        IIntStreamHandle scopeTimestamp = self->scope->getTimestamp(self->scope);
        self->packer->prepareTimestamp(self->packer, scopeTimestamp);

        const uint32_t timeIncrement = self->scope->getTimeIncrement(self->scope);
        self->packer->prepareTimeIncrement(self->packer, timeIncrement);

    }
    if(self->scope->announcementIsReadyToSend(self->scope)){
        const size_t maxAddresses = self->scope->getMaxAmmountOfAnnounceAddresses(self->scope);

        for(uint32_t i = 0; i < maxAddresses; ++i){
            AddressDefinition* addr = self->scope->getAnnounceAddressToTransmit(self->scope, i);

            if(addr != NULL){
                if(addr->hasToBeSent == true){
                    self->packer->prepareAddressAnnouncement(self->packer, addr->name, getDataTypeName(addr->type),
                                                             addr->address);
                }
            }
        }
    }

    self->scope->dataIsTransmitted(self->scope);
}

/******************************************************************************
 Private functions
******************************************************************************/
CommandPackHandle CommandPack_create(IScopeHandle scope, IPackerHandle packer){

    CommandPackHandle self = malloc(sizeof(CommandPackPrivateData));
    self->scope = scope;
    self->packer = packer;

    const size_t maxSizeOfChannels = self->scope->getMaxSizeOfChannel(self->scope);
    const size_t amountOfChannels = self->scope->getAmountOfChannels(self->scope);

    // Todo: this takes up unnecessary space. Maybe streams would be a better option?
    self->channelData = malloc(sizeof(float**) * amountOfChannels);

    for(size_t i = 0; i < amountOfChannels; i++){
        self->channelData[i] = malloc(sizeof(float*) * maxSizeOfChannels);
    }

    self->command.handle = self;
    self->command.run = &run;

    return self;
}

ICommandHandle CommandPack_getICommand(CommandPackHandle self){
    return &self->command;
}

void CommandPack_destroy(CommandPackHandle self){
    free(self->channelData);
    self = NULL;
    free(self);
    self = NULL;
}
