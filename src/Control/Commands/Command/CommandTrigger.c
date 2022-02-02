/*!****************************************************************************************************************************************
 * @file         CommandTrigger.c
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

#include "Scope/Control/Commands/Command/CommandTrigger.h"
#include "Scope/Control/ICommand.h"
#include "Scope/Core/IScope.h"
#include "Scope/Core/ScopeTypes.h"

#include <stdlib.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandTriggerPrivateData{
    ICommand command;

    IScopeHandle scope;
    TriggerConfiguration config;
    IObserverHandle observer;

} CommandTriggerPrivateData;

static void run(ICommandHandle command);

/******************************************************************************
 Private functions
******************************************************************************/
static void run(ICommandHandle command){
    CommandTriggerHandle self = (CommandTriggerHandle) command->handle;
    self->scope->configureTrigger(self->scope, self->config);
    MessageType resp = SE_ACK;
    self->observer->update(self->observer, &resp);
}

/******************************************************************************
Public functions
******************************************************************************/
CommandTriggerHandle CommandTrigger_create(IScopeHandle scope, IObserverHandle observer){

    CommandTriggerHandle self = malloc(sizeof(CommandTriggerPrivateData));
    assert(self);

    self->command.handle = self;
    self->scope = scope;
    self->command.run = &run;
    self->observer = observer;

    return self;
}

ICommandHandle CommandTrigger_getICommand(CommandTriggerHandle self){
    return &self->command;
}

void CommandTrigger_setAttributes(CommandTriggerHandle self, TriggerConfiguration conf){

    self->config = conf;
}

void CommandTrigger_destroy(CommandTriggerHandle self){
    free(self);
    self = NULL;
}
