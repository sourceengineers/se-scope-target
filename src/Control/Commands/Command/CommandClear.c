/*!****************************************************************************************************************************************
 * @file         CommandClear.c
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

#include "Scope/Control/Commands/Command/CommandClear.h"
#include "Scope/Control/ICommand.h"
#include "Scope/Core/IScope.h"

#include <stdlib.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandClearPrivateData{
    ICommand command;

    IScopeHandle scope;
    IObserverHandle observer;

} CommandClearPrivateData;

static void run(ICommandHandle command);

/******************************************************************************
 Private functions
******************************************************************************/
static void run(ICommandHandle command){
    CommandClearHandle self = (CommandClearHandle) command->handle;
    self->scope->clear(self->scope);
    MessageType resp = SE_ACK;
    self->observer->update(self->observer, &resp);
}

/******************************************************************************
 Private functions
******************************************************************************/
CommandClearHandle CommandClear_create(IScopeHandle scope, IObserverHandle observer){

    CommandClearHandle self = malloc(sizeof(CommandClearPrivateData));
    assert(self);

    self->scope = scope;
    self->observer = observer;

    self->command.handle = self;
    self->command.run = &run;

    return self;
}

ICommandHandle CommandClear_getICommand(CommandClearHandle self){
    return &self->command;
}

void CommandClear_destroy(CommandClearHandle self){
    free(self);
    self = NULL;
}
