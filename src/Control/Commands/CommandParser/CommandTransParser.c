/*!****************************************************************************************************************************************
 * @file         CommandTransParser.c
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

#include "Scope/Control/Commands/CommandParser/CommandTransParser.h"
#include "Scope/Control/Commands/Command/CommandTrans.h"

#include <stdlib.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandTransParserPrivateData{
    CommandTransHandle command;

} CommandTransParserPrivateData;

/******************************************************************************
 Public functions
******************************************************************************/
CommandTransParserHandle CommandTransParser_create(IScopeHandle scope, IObserverHandle observer){
    CommandTransParserHandle self = malloc(sizeof(CommandTransParserPrivateData));
    assert(self);

    self->command = CommandTrans_create(scope, observer);
    return self;
}

ICommandHandle CommandTransParser_getCommand(CommandTransParserHandle self){
    return CommandTrans_getICommand(self->command);
}

void CommandTransParser_destroy(CommandTransParserHandle self){
    CommandTrans_destroy(self->command);

    free(self);
    self = NULL;
}
