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
#include <Scope/Core/ScopeTypes.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandPackAnnouncePrivateData{
    ICommand command;

    IScopeHandle scope;
    IPackerHandle packer;

} CommandPackAnnouncePrivateData;

static void run(ICommandHandle command);

/******************************************************************************
 Private functions
******************************************************************************/
static void run(ICommandHandle command){
    CommandPackAnnounceHandle self = (CommandPackAnnounceHandle) command->handle;

    const size_t maxAddresses = self->scope->getMaxAmmountOfAnnounceAddresses(self->scope);

    for(uint32_t i = 0; i < maxAddresses; ++i){
        AddressDefinition* addr = self->scope->getAnnounceAddressToTransmit(self->scope, i);

        if(addr != NULL){
            self->packer->prepareAddressAnnouncement(self->packer, addr->name, getDataTypeName(addr->type),
                                                     addr->address);
        }
    }
}

/******************************************************************************
 Private functions
******************************************************************************/
CommandPackAnnounceHandle CommandPackAnnounce_create(IScopeHandle scope, IPackerHandle packer){

    CommandPackAnnounceHandle self = malloc(sizeof(CommandPackAnnouncePrivateData));
    self->scope = scope;
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
