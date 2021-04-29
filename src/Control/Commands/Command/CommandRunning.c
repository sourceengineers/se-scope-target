/*!****************************************************************************************************************************************
 * @file         CommandRunning.c
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

#include "Scope/Control/Commands/Command/CommandRunning.h"
#include "Scope/Control/ICommand.h"
#include "Scope/Core/IScope.h"

#include <stdlib.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/

/* Class data */
typedef struct __CommandRunningPrivateData{
    ICommand command;

    CommandRunningConf config;
    IScopeHandle scope;

    size_t amountOfChannels;
    IObserverHandle observer;

} CommandRunningPrivateData;

static void run(ICommandHandle command);

/******************************************************************************
 Private functions
******************************************************************************/
static void run(ICommandHandle command){
    CommandRunningHandle self = (CommandRunningHandle) command->handle;

    for(size_t i = 0; i < self->config.numberOfChangedChannels; i++){
        const uint32_t idOfChannelChanged = self->config.changedChannels[i];

        if(idOfChannelChanged >= self->amountOfChannels){
            return;
        }

        if(self->config.newStates[i] == CHANNEL_RUNNING){
            self->scope->setChannelRunning(self->scope, idOfChannelChanged);
        }else if(self->config.newStates[i] == CHANNEL_STOPPED){
            self->scope->setChannelStopped(self->scope, idOfChannelChanged);
        }
    }
    MessageType resp = SE_ACK;
    self->observer->update(self->observer, &resp);
}

/******************************************************************************
 Public functions
******************************************************************************/
CommandRunningHandle CommandRunning_create(IScopeHandle scope, IObserverHandle observer){

    CommandRunningHandle self = malloc(sizeof(CommandRunningPrivateData));
    assert(self);

    self->scope = scope;
    self->amountOfChannels = self->scope->getAmountOfChannels(self->scope);

    self->config.newStates = malloc(sizeof(CHANNEL_STATES) * self->amountOfChannels);
    assert(self->config.newStates);
    self->config.changedChannels = malloc(sizeof(int) * self->amountOfChannels);
    assert(self->config.changedChannels);
    self->config.numberOfChangedChannels = 0;
    self->observer = observer;

    self->command.handle = self;
    self->command.run = &run;

    return self;
}

ICommandHandle CommandRunning_getICommand(CommandRunningHandle self){
    return &self->command;
}

void CommandRunning_setAttributes(CommandRunningHandle self, CommandRunningConf conf){

    /* Safety checks */
    if(conf.numberOfChangedChannels > self->amountOfChannels){
        return;
    }

    for(size_t i = 0; i < conf.numberOfChangedChannels; i++){
        if(conf.changedChannels[i] > self->amountOfChannels){
            return;
        }
        if(conf.newStates[i] != CHANNEL_STOPPED
           && conf.newStates[i] != CHANNEL_RUNNING){
            return;
        }
    }

    /* Copy data to command */
    self->config.numberOfChangedChannels = conf.numberOfChangedChannels;
    for(size_t i = 0; i < conf.numberOfChangedChannels; i++){
        self->config.changedChannels[i] = conf.changedChannels[i];
        self->config.newStates[i] = conf.newStates[i];
    }
}

void CommandRunning_destroy(CommandRunningHandle self){
    free(self->config.newStates);
    self->config.newStates = NULL;
    free(self->config.changedChannels);
    self->config.changedChannels = NULL;
    free(self);
    self = NULL;
}
