/*!****************************************************************************************************************************************
 * @file         CommandPackAnnounce.c
 *
 * @copyright    Copyright (c) 2021 by Source Engineers GmbH. All Rights Reserved.
 *
 * @license {    This file is part of se-scope-target.
 *
 *               se-scope-target is free software; you can redistribute it and/or
 *               modify it under the terms of the GPLv3 General Public License Version 3
 *               as published by the Free Software Foundation.
 *
 *               se-scope-target is distributed in the hope that it will be useful,
 *               but WITHOUT ANY WARRANTY; without even the implied warranty of
 *               MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *               GNU General Public License for more details.
 *
 *               You should have received a copy of the GPLv3 General Public License Version 3
 *               along with se-scope-target.  If not, see <http://www.gnu.org/licenses/>.
 *
 *               In closed source or commercial projects, GPLv3 General Public License Version 3
 *               is not valid. In this case the commercial license received with the purchase
 *               is applied (See SeScopeLicense.pdf).
 *               Please contact us at scope@sourceengineers.com for a commercial license.
 * }
 *
 * @authors      Samuel Schuepbach <samuel.schuepbach@sourceengineers.com>
 *
*****************************************************************************************************************************************/

#include "Scope/GeneralPurpose/DataTypes.h"

#include "Scope/Control/PackCommands/Command/CommandPackAnnounce.h"
#include "Scope/Control/IPacker.h"
#include "Scope/Control/ICommand.h"
#include "Scope/Core/ScopeTypes.h"
#include "Scope/Core/IScope.h"
#include "Scope/Version.h"

#include <stdint.h>
#include <stdlib.h>
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
        ScAnnounceChannelDef address = {.type = addr->type, .id = i, .name = addr->name, .address = addr->address};
        self->packer->addAddressAnnouncement(self->packer, address);
    }
    
    const size_t maxAmountOfChannels = AnnounceStorage_getMaxAmountOfChannels(self->announceStorage);
    const float timeBase = AnnounceStorage_getTimeBase(self->announceStorage);

    char* version = AnnounceStorage_getVersion(self->announceStorage);

    ScAnnounceMetaData meta = {.timebase = timeBase, .version = version, .maxChannels = maxAmountOfChannels};
    self->packer->addAnnouncement(self->packer, meta);
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
