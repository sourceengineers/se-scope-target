/*!****************************************************************************************************************************************
 * @file         CommandAddrParser.c
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

#include "Scope/Control/Commands/CommandParser/CommandAddrParser.h"
#include "Scope/Control/Commands/Command/CommandAddr.h"
#include "Scope/Control/IUnpacker.h"
#include "Scope/Core/ScopeTypes.h"
#include "Scope/Control/ParserDefinitions.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_CHANNELS 10

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandAddrParserPrivateData{
    uint32_t* channelIds;
    void** newAddresses;
    DATA_TYPES* types;
    uint32_t amountOfChannels;
    CommandAddrHandle command;
    IUnpackerHandle unpacker;
} CommandAddrParserPrivateData;

/******************************************************************************
 Private functions
******************************************************************************/

/******************************************************************************
 Public functions
******************************************************************************/
CommandAddrParserHandle
CommandAddrParser_create(IScopeHandle scope,
                         IUnpackerHandle unpacker,
                         IObserverHandle observer,
                         size_t amountOfChannels) {
    CommandAddrParserHandle self = malloc(sizeof(CommandAddrParserPrivateData));
    assert(self);

    self->channelIds = (uint32_t*) malloc(sizeof(uint32_t) * amountOfChannels);
    assert(self->channelIds != NULL);
    self->newAddresses = (void**) malloc(sizeof(void*) * amountOfChannels);
    assert(self->newAddresses != NULL);
    self->types = (DATA_TYPES*) malloc(sizeof(DATA_TYPES) * amountOfChannels);
    assert(self->types != NULL);

    self->command = CommandAddr_create(scope, observer);
    self->unpacker = unpacker;
    self->amountOfChannels = amountOfChannels;

    return self;
}

ICommandHandle CommandAddrParser_getCommand(CommandAddrParserHandle self){

    if(self->unpacker == NULL){
        return NULL;
    }

    size_t amount = self->unpacker->cfAddress_getAmount(self->unpacker);

    if (amount > self->amountOfChannels) {
        amount = self->amountOfChannels;
    }

    for(uint32_t i = 0; i < amount; i++){
        CfAddressDef addr = self->unpacker->cfAddress_getChannel(self->unpacker, i);
        self->newAddresses[i] = (void*) (ADDRESS_DATA_TYPE) addr.address;
        self->channelIds[i] = addr.id;
        self->types[i] = addr.type;
    }

    CommandAddrConf conf = {.newAddresses = self->newAddresses, \
                          .changedChannels = self->channelIds, \
                          .types = self->types, \
                          .numberOfChangedChannels = amount};

    CommandAddr_setAttributes(self->command, conf);

    return CommandAddr_getICommand(self->command);
}

/* Deconstructor: Deletes the instance of the channel */
void CommandAddrParser_destroy(CommandAddrParserHandle self){
    CommandAddr_destroy(self->command);
    free(self->channelIds);
    free(self->newAddresses);
    free(self->types);
    free(self);
    self = NULL;
}
