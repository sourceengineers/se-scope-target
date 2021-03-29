/*!****************************************************************************************************************************************
 * @file         CommandPollParser.c
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

#include "Scope/Control/Commands/CommandParser/CommandPollParser.h"
#include "Scope/Control/Commands/Command/CommandPoll.h"

#include <stdlib.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandPollParserPrivateData{
    CommandPollHandle command;

} CommandPollParserPrivateData;

/******************************************************************************
 Public functions
******************************************************************************/
CommandPollParserHandle CommandPollParser_create(IScopeHandle scope, IObserverHandle observer){
    CommandPollParserHandle self = malloc(sizeof(CommandPollParserPrivateData));
    assert(self);

    self->command = CommandPoll_create(scope, observer);
    return self;
}

ICommandHandle CommandPollParser_getCommand(CommandPollParserHandle self){

    return CommandPoll_getICommand(self->command);
}

void CommandPollParser_destroy(CommandPollParserHandle self){
    CommandPoll_destroy(self->command);

    free(self);
    self = NULL;
}
