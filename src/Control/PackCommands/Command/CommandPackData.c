/*!****************************************************************************************************************************************
 * @file         CommandPackData.c
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
 *               is not valid. In these cases the purchased commercial license is applied.
 *               Please contact us at scope@sourceengineers.com for a commercial license.
 * }
 *
 * @authors      Samuel Schuepbach <samuel.schuepbach@sourceengineers.com>
 *
*****************************************************************************************************************************************/

#include "Scope/Control/PackCommands/Command/CommandPackData.h"
#include "Scope/Control/IPacker.h"
#include "Scope/Control/ICommand.h"
#include "Scope/Core/ScopeTypes.h"
#include "Scope/Core/IScope.h"
#include <se-lib-c/container/FloatRingBuffer.h>
#include <se-lib-c/stream/IIntStream.h>
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
typedef struct __CommandPackDataPrivateData{
    ICommand command;

    IScopeHandle scope;
    IPackerHandle packer;

} CommandPackDataPrivateData;

static void run(ICommandHandle command);

/******************************************************************************
 Private functions
******************************************************************************/

static void run(ICommandHandle command){
    CommandPackDataHandle self = (CommandPackDataHandle) command->handle;

    bool anyChannelIsReady = false;

    for(uint32_t i = 0; i < self->scope->getAmountOfChannels(self->scope); ++i){
        if(self->scope->channelHasToBePacked(self->scope, i) == true){
            FloatRingBufferHandle buffer = self->scope->getChannelBuffer(self->scope, i);
            ScDataChannelDef channel;
            channel.stream = buffer;
            channel.id = i;
            self->packer->addChannel(self->packer, channel);
            anyChannelIsReady = true;
        }
    }

    if(anyChannelIsReady == false){
        return;
    }

    TriggeredValues triggeredValues = self->scope->getTriggerData(self->scope);

    ScDataTriggerDef trigger;
    trigger.timestamp = triggeredValues.triggerTimestamp;
    trigger.channelId = triggeredValues.channelId;
    trigger.triggerMode = triggeredValues.mode;
    trigger.isTriggered = triggeredValues.isTriggered;

    self->packer->addTrigger(self->packer, trigger);

    IIntStreamHandle scopeTimestamp = self->scope->getTimestamp(self->scope);
    self->packer->addTimestamp(self->packer, scopeTimestamp);

    const uint32_t timeIncrement = self->scope->getTimeIncrement(self->scope);
    self->packer->addTimeIncrement(self->packer, timeIncrement);
}

/******************************************************************************
 Private functions
******************************************************************************/
CommandPackDataHandle CommandPackData_create(IScopeHandle scope, IPackerHandle packer){

    CommandPackDataHandle self = malloc(sizeof(CommandPackDataPrivateData));
    assert(self);

    self->scope = scope;
    self->packer = packer;

    self->command.handle = self;
    self->command.run = &run;

    return self;
}

ICommandHandle CommandPackData_getICommand(CommandPackDataHandle self){
    return &self->command;
}

void CommandPackData_destroy(CommandPackDataHandle self){
    free(self);
    self = NULL;
}
