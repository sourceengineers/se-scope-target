/*!****************************************************************************************************************************************
 * @file         CommandAddrParser.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
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

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandAddrParserPrivateData{
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
CommandAddrParser_create(IScopeHandle scope, IUnpackerHandle unpacker, IObserverHandle observer){
    CommandAddrParserHandle self = malloc(sizeof(CommandAddrParserPrivateData));
    assert(self);

    self->command = CommandAddr_create(scope, observer);
    self->unpacker = unpacker;
    return self;
}

ICommandHandle CommandAddrParser_getCommand(CommandAddrParserHandle self){

    if(self->unpacker == NULL){
        return NULL;
    }

    const size_t amount = self->unpacker->cfAddress_getAmount(self->unpacker);

    uint32_t channelIds[amount];
    void* newAddresses[amount];
    DATA_TYPES types[amount];

    for(uint32_t i = 0; i < amount; i++){
        CfAddressDef addr = self->unpacker->cfAddress_getChannel(self->unpacker, i);
        newAddresses[i] = (void*) (ADDRESS_DATA_TYPE) addr.address;
        channelIds[i] = addr.id;
        types[i] = addr.type;
    }

    CommandAddrConf conf = {.newAddresses = newAddresses, \
                          .changedChannels = channelIds, \
                          .types = types, \
                          .numberOfChangedChannels = amount};

    CommandAddr_setAttributes(self->command, conf);

    return CommandAddr_getICommand(self->command);
}

/* Deconstructor: Deletes the instance of the channel */
void CommandAddrParser_destroy(CommandAddrParserHandle self){
    CommandAddr_destroy(self->command);

    free(self);
    self = NULL;
}
