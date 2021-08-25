/*!****************************************************************************************************************************************
 * @file         CommandAddr.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include "Scope/GeneralPurpose/DataTypes.h"

#include "Scope/Control/Commands/Command/CommandAddr.h"
#include "Scope/Control/ICommand.h"
#include "Scope/Core/IScope.h"

#include <stdlib.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandAddrPrivateData{
    ICommand command;

    CommandAddrConf config;
    IScopeHandle scope;
    IObserverHandle observer;
    size_t amountOfChannels;

} CommandAddrPrivateData;

static void run(ICommandHandle command);

/******************************************************************************
 Private functions
******************************************************************************/
static void run(ICommandHandle command){
    CommandAddrHandle self = (CommandAddrHandle) command->handle;

    /* Set the polling address of the channels */
    for(size_t i = 0; i < self->config.numberOfChangedChannels; i++){
        const uint32_t idChangingChannel = self->config.changedChannels[i];

        if(idChangingChannel >= self->amountOfChannels){
            return;
        }

        const DATA_TYPES newType = self->config.types[i];
        void* newAddress = self->config.newAddresses[i];

        self->scope->configureChannelAddress(self->scope, newAddress, idChangingChannel, newType);
    }
    MessageType resp = SE_ACK;
    self->observer->update(self->observer, &resp);
}

/******************************************************************************
 Public functions
******************************************************************************/
CommandAddrHandle CommandAddr_create(IScopeHandle scope, IObserverHandle observer){

    CommandAddrHandle self = malloc(sizeof(CommandAddrPrivateData));
    assert(self);
    self->scope = scope;
    self->observer = observer;
    self->amountOfChannels = self->scope->getAmountOfChannels(self->scope);
    self->config.newAddresses = malloc(sizeof(void*) * self->amountOfChannels);
    assert(self->config.newAddresses);
    self->config.changedChannels = malloc(sizeof(int) * self->amountOfChannels);
    assert(self->config.changedChannels);
    self->config.types = malloc(sizeof(DATA_TYPES) * self->amountOfChannels);
    assert(self->config.types);
    self->config.numberOfChangedChannels = 0;

    self->command.handle = self;
    self->command.run = &run;

    return self;
}

ICommandHandle CommandAddr_getICommand(CommandAddrHandle self){
    return &self->command;
}

void CommandAddr_setAttributes(CommandAddrHandle self, CommandAddrConf conf){

    /* Safety checks */
    if(conf.numberOfChangedChannels > self->amountOfChannels){
        return;
    }

    /* Check that no id is bigger than the maximum ammount of channels */
    for(size_t i = 0; i < conf.numberOfChangedChannels; i++){
        if(conf.changedChannels[i] > self->amountOfChannels){
            return;
        }
        if(conf.newAddresses[i] == NULL){
            return;
        }
    }

    /* Copy data to command */
    self->config.numberOfChangedChannels = conf.numberOfChangedChannels;
    for(size_t i = 0; i < conf.numberOfChangedChannels; i++){
        self->config.changedChannels[i] = conf.changedChannels[i];
        self->config.newAddresses[i] = conf.newAddresses[i];
        self->config.types[i] = conf.types[i];
    }
}

void CommandAddr_destroy(CommandAddrHandle self){
    free(self->config.newAddresses);
    self->config.newAddresses = NULL;
    free(self->config.changedChannels);
    self->config.changedChannels = NULL;
    free(self->config.types);
    self->config.types = NULL;
    free(self);
    self = NULL;
}
