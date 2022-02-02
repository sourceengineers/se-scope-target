/*!****************************************************************************************************************************************
 * @file         CommandAnnounce.c
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

#include "Scope/Control/Commands/Command/CommandAnnounce.h"
#include "Scope/Control/ICommand.h"
#include "Scope/Core/IScope.h"

#include <stdlib.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandAnnouncePrivateData{
    ICommand command;

    IObserverHandle packObserver;

} CommandAnnouncePrivateData;

static void run(ICommandHandle command);

/******************************************************************************
 Private functions
******************************************************************************/
static void run(ICommandHandle command){
    CommandAnnounceHandle self = (CommandAnnounceHandle) command->handle;
    MessageType packType = SC_ANNOUNCE;
    self->packObserver->update(self->packObserver, &packType);
}

/******************************************************************************
 Private functions
******************************************************************************/
CommandAnnounceHandle CommandAnnounce_create(IObserverHandle packObserver){

    CommandAnnounceHandle self = malloc(sizeof(CommandAnnouncePrivateData));
    assert(self);
    self->packObserver = packObserver;

    self->command.handle = self;
    self->command.run = &run;

    return self;
}

ICommandHandle CommandAnnounce_getICommand(CommandAnnounceHandle self){
    return &self->command;
}

void CommandAnnounce_destroy(CommandAnnounceHandle self){
    free(self);
    self = NULL;
}
