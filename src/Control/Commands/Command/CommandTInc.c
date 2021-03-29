/*!****************************************************************************************************************************************
 * @file         CommandTInc.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @license      This file is part of SE-Scope.
 *
 *               SE-Scope is free software; you can redistribute it and/or
 *               modify it under the terms of the GNU General Public License as
 *               published by the Free Software Foundation.
 *
 *               SE-Scope is distributed in the hope that it will be useful,
 *               but WITHOUT ANY WARRANTY; without even the implied warranty of
 *               MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *               GNU General Public License for more details.
 *
 *               You should have received a copy of the GNU General Public License
 *               along with SE-Scope.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include "Scope/Control/Commands/Command/CommandTInc.h"
#include "Scope/Control/ICommand.h"
#include "Scope/Core/IScope.h"

#include <stdlib.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/

/* Class data */
typedef struct __CommandTIncPrivateData{
    ICommand command;

    IScopeHandle scope;
    IObserverHandle observer;

    uint32_t timeIncrement;
} CommandTIncPrivateData;

static void run(ICommandHandle command);

/******************************************************************************
 Private functions
******************************************************************************/
static void run(ICommandHandle command){
    CommandTIncHandle self = (CommandTIncHandle) command->handle;
    self->scope->setTimeIncrement(self->scope, self->timeIncrement);
    MessageType resp = SE_ACK;
    self->observer->update(self->observer, &resp);
}

/******************************************************************************
 Private functions
******************************************************************************/
CommandTIncHandle CommandTInc_create(IScopeHandle scope, IObserverHandle observer){

    CommandTIncHandle self = malloc(sizeof(CommandTIncPrivateData));
    assert(self);

    self->scope = scope;
    self->timeIncrement = 0;

    self->observer = observer;
    self->command.handle = self;
    self->command.run = &run;

    return self;
}

ICommandHandle CommandTInc_getICommand(CommandTIncHandle self){
    return &self->command;
}

void CommandTInc_setAttributes(CommandTIncHandle self, uint32_t timeIncrement){
    self->timeIncrement = timeIncrement;
}

void CommandTInc_destroy(CommandTIncHandle self){
    free(self);
    self = NULL;
}
