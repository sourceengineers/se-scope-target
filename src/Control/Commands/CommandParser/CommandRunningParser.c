/*!****************************************************************************************************************************************
 * @file         CommandRunningParser.c
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
    uint32_t* channelIds;
    CHANNEL_STATES* newStates;
    CommandRunningHandle command;
    IUnpackerHandle unpacker;
} CommandRunningParserPrivateData;

/******************************************************************************
 Public functions
******************************************************************************/
CommandRunningParserHandle CommandRunningParser_create(IScopeHandle scope,
                                                       IUnpackerHandle unpacker,
                                                       IObserverHandle observer,
                                                       size_t amountOfChannels) {
    CommandRunningParserHandle self = malloc(sizeof(CommandRunningParserPrivateData));
    assert(self);
    self->channelIds = (uint32_t*) malloc(sizeof(uint32_t) * amountOfChannels);
    assert(self->channelIds != NULL);
    self->newStates = (CHANNEL_STATES*) malloc(sizeof(CHANNEL_STATES) * amountOfChannels);
    assert(self->newStates != NULL);

    self->command = CommandRunning_create(scope, observer);
    self->unpacker = unpacker;
    return self;
}

ICommandHandle CommandRunningParser_getCommand(CommandRunningParserHandle self){

    if(self->unpacker == NULL){
        return NULL;
    }

    size_t amount = self->unpacker->cfRunning_getAmount(self->unpacker);

    for(size_t i = 0; i < amount; ++i){

        CfRunningDef running = self->unpacker->cfRunning_getChannel(self->unpacker, i);
        self->channelIds[i] = running.id;
        if(running.newState == true){
            self->newStates[i] = CHANNEL_RUNNING;
        }else{
            self->newStates[i] = CHANNEL_STOPPED;
        }
    }

    CommandRunningConf conf = {.newStates = self->newStates, \
                          .changedChannels = self->channelIds, \
                          .numberOfChangedChannels = amount};

    CommandRunning_setAttributes(self->command, conf);

    return CommandRunning_getICommand(self->command);
}

void CommandRunningParser_destroy(CommandRunningParserHandle self){
    CommandRunning_destroy(self->command);
    free(self->newStates);
    free(self->channelIds);
    free(self);
    self = NULL;
}
