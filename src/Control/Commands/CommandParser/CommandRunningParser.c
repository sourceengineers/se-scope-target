/*!****************************************************************************************************************************************
 * @file         CommandRunningParser.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include "Scope/Control/Commands/CommandParser/CommandRunningParser.h"
#include "Scope/Control/Commands/Command/CommandRunning.h"
#include "Scope/Control/IUnpacker.h"
#include "Scope/Core/ScopeTypes.h"
#include "Scope/Control/ParserDefinitions.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandRunningParserPrivateData{
    CommandRunningHandle command;
    IUnpackerHandle unpacker;

} CommandRunningParserPrivateData;

/******************************************************************************
 Public functions
******************************************************************************/
CommandRunningParserHandle CommandRunningParser_create(IScopeHandle scope, IUnpackerHandle unpacker,
        IObserverHandle observer){
    CommandRunningParserHandle self = malloc(sizeof(CommandRunningParserPrivateData));
    assert(self);
    self->command = CommandRunning_create(scope, observer);
    self->unpacker = unpacker;
    return self;
}

ICommandHandle CommandRunningParser_getCommand(CommandRunningParserHandle self){

    if(self->unpacker == NULL){
        return NULL;
    }

    const size_t amount = self->unpacker->cfRunning_getAmount(self->unpacker);

    uint32_t channelIds[amount];
    CHANNEL_STATES newStates[amount];

    for(size_t i = 0; i < amount; ++i){

        CfRunningDef running = self->unpacker->cfRunning_getChannel(self->unpacker, i);
        channelIds[i] = running.id;
        if(running.newState == true){
            newStates[i] = CHANNEL_RUNNING;
        }else{
            newStates[i] = CHANNEL_STOPPED;
        }
    }

    CommandRunningConf conf = {.newStates = newStates, \
                          .changedChannels = channelIds, \
                          .numberOfChangedChannels = amount};

    CommandRunning_setAttributes(self->command, conf);

    return CommandRunning_getICommand(self->command);
}

void CommandRunningParser_destroy(CommandRunningParserHandle self){
    CommandRunning_destroy(self->command);

    free(self);
    self = NULL;
}
