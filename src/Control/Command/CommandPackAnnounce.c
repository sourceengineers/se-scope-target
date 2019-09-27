/*!****************************************************************************************************************************************
* @file         CommandPackAnnounce.c
*
* @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
*
* @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
*
*****************************************************************************************************************************************/

#include <Scope/Control/Command/CommandPackAnnounce.h>
#include <Scope/Control/IPacker.h>
#include <Scope/Control/Command/ICommand.h>
#include <Scope/Core/ScopeTypes.h>
#include <Scope/Core/IScope.h>
#include <Scope/GeneralPurpose/DataTypes.h>

#include <stdint.h>
#include <stdlib.h>
#include <Version.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandPackAnnouncePrivateData{
    ICommand command;

    AnnounceStorageHandle announceStorage;
    IPackerHandle packer;

} CommandPackAnnouncePrivateData;

static void run(ICommandHandle command);

/******************************************************************************
 Private functions
******************************************************************************/
static void run(ICommandHandle command){
    CommandPackAnnounceHandle self = (CommandPackAnnounceHandle) command->handle;

    if(self->announceStorage == NULL){
        return;
    }

    const size_t maxAddresses = AnnounceStorage_getMaxAmountOfAddresses(self->announceStorage);

    for(uint32_t i = 0; i < maxAddresses; ++i){
        AddressDefinition* addr = AnnounceStorage_getAddressToTransmit(self->announceStorage, i);

        if(addr != NULL){
            self->packer->prepareAddressAnnouncement(self->packer, addr->name, getDataTypeName(addr->type),
                                                     addr->address);
        }
    }
    
    const size_t maxAmountOfChannels = AnnounceStorage_getMaxAmountOfChannels(self->announceStorage);
    const float timeBase = AnnounceStorage_getTimeBase(self->announceStorage);

    char version[SE_SCOPE_TARGET_VERSION_LENGTH];
    AnnounceStorage_getVersion(self->announceStorage, version);

    self->packer->prepareAnnouncement(self->packer, timeBase, version, maxAmountOfChannels);
}

/******************************************************************************
 Private functions
******************************************************************************/
CommandPackAnnounceHandle CommandPackAnnounce_create(AnnounceStorageHandle announceStorage, IPackerHandle packer){

    CommandPackAnnounceHandle self = malloc(sizeof(CommandPackAnnouncePrivateData));
    assert(self);

    self->announceStorage = announceStorage;
    self->packer = packer;

    self->command.handle = self;
    self->command.run = &run;

    return self;
}

ICommandHandle CommandPackAnnounce_getICommand(CommandPackAnnounceHandle self){
    return &self->command;
}

void CommandPackAnnounce_destroy(CommandPackAnnounceHandle self){
    free(self);
    self = NULL;
}
