/*!****************************************************************************************************************************************
 * @file         CommandDetect.c
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

#include "Scope/Control/Commands/Command/CommandDetect.h"
#include "Scope/Control/ICommand.h"
#include "Scope/Core/IScope.h"

#include <stdlib.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandDetectPrivateData{
    ICommand command;

    IObserverHandle packObserver;

} CommandDetectPrivateData;

static void run(ICommandHandle command);

/******************************************************************************
 Private functions
******************************************************************************/
static void run(ICommandHandle command){
    CommandDetectHandle self = (CommandDetectHandle) command->handle;
    MessageType packType = SC_DETECT;
    self->packObserver->update(self->packObserver, &packType);
}

/******************************************************************************
 Private functions
******************************************************************************/
CommandDetectHandle CommandDetect_create(IObserverHandle packObserver){

    CommandDetectHandle self = malloc(sizeof(CommandDetectPrivateData));
    assert(self);

    self->packObserver = packObserver;

    self->command.handle = self;
    self->command.run = &run;

    return self;
}

ICommandHandle CommandDetect_getICommand(CommandDetectHandle self){
    return &self->command;
}

void CommandDetect_destroy(CommandDetectHandle self){
    free(self);
    self = NULL;
}
