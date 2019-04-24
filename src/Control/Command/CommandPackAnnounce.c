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

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandPackAnnouncePrivateData{
    ICommand command;

    AddressStorageHandle addressStorage;
    IPackerHandle packer;

} CommandPackAnnouncePrivateData;

static void run(ICommandHandle command);

/******************************************************************************
 Private functions
******************************************************************************/
static void run(ICommandHandle command){
    CommandPackAnnounceHandle self = (CommandPackAnnounceHandle) command->handle;

    if(self->addressStorage == NULL){
        return;
    }

    const size_t maxAddresses = AddressStorage_getMaxAmountOfAddresses(self->addressStorage);

    for(uint32_t i = 0; i < maxAddresses; ++i){
        AddressDefinition* addr = AddressStorage_getAddressToTransmit(self->addressStorage, i);

        if(addr != NULL){
            self->packer->prepareAddressAnnouncement(self->packer, addr->name, getDataTypeName(addr->type),
                                                     addr->address);
        }
    }
}

/******************************************************************************
 Private functions
******************************************************************************/
CommandPackAnnounceHandle CommandPackAnnounce_create(AddressStorageHandle addressStorage, IPackerHandle packer){

    CommandPackAnnounceHandle self = malloc(sizeof(CommandPackAnnouncePrivateData));
    self->addressStorage = addressStorage;
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