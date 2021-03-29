/*!****************************************************************************************************************************************
 * @file         CommandClearParser.c
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

#include "Scope/Control/Commands/CommandParser/CommandClearParser.h"
#include "Scope/Control/Commands/Command/CommandClear.h"

#include <stdlib.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandClearParserPrivateData{
    CommandClearHandle command;

} CommandClearParserPrivateData;

/******************************************************************************
 Public functions
******************************************************************************/
CommandClearParserHandle CommandClearParser_create(IScopeHandle scope, IObserverHandle packObserver){
    CommandClearParserHandle self = malloc(sizeof(CommandClearParserPrivateData));
    assert(self);

    self->command = CommandClear_create(scope, packObserver);
    return self;
}

ICommandHandle CommandClearParser_getCommand(CommandClearParserHandle self){
    return CommandClear_getICommand(self->command);
}

void CommandClearParser_destroy(CommandClearParserHandle self){
    CommandClear_destroy(self->command);

    free(self);
    self = NULL;
}
