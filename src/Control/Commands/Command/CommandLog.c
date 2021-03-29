/*!****************************************************************************************************************************************
 * @file         CommandLog.c
 *
 * @copyright    Copyright (c) 2020 by Sourceengineers. All Rights Reserved.
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
 * @authors      Anselm Fuhrer anselm.fuhrer@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include "Scope/Control/Commands/Command/CommandLog.h"
#include "Scope/Control/ICommand.h"
#include "Scope/Core/IScope.h"

#include <stdlib.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandLogPrivateData{
    ICommand command;

    IObserverHandle packObserver;

} CommandLogPrivateData;

static void run(ICommandHandle command);

/******************************************************************************
 Private functions
******************************************************************************/
static void run(ICommandHandle command){
    CommandLogHandle self = (CommandLogHandle) command->handle;
    MessageType packType = SC_LOG;
    self->packObserver->update(self->packObserver, &packType);
}

/******************************************************************************
 Private functions
******************************************************************************/
CommandLogHandle CommandLog_create(IObserverHandle packObserver){

	CommandLogHandle self = malloc(sizeof(CommandLogPrivateData));
    assert(self);
    self->packObserver = packObserver;

    self->command.handle = self;
    self->command.run = &run;

    return self;
}

ICommandHandle CommandLog_getICommand(CommandLogHandle self){
    return &self->command;
}

void CommandLog_destroy(CommandLogHandle self){
    free(self);
    self = NULL;
}
