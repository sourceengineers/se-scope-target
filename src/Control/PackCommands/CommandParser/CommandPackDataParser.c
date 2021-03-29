/*!****************************************************************************************************************************************
 * @file         CommandPackDataParser.c
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

#include "Scope/Control/PackCommands/CommandParser/CommandPackDataParser.h"
#include "Scope/Control/PackCommands/Command/CommandPackData.h"

#include <stdlib.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandPackDataParserPrivateData{
    CommandPackDataHandle command;

} CommandPackDataParserPrivateData;

/******************************************************************************
 Public functions
******************************************************************************/
CommandPackDataParserHandle CommandPackDataParser_create(IScopeHandle scope, IPackerHandle packer){
    CommandPackDataParserHandle self = malloc(sizeof(CommandPackDataParserPrivateData));
    assert(self);
    self->command = CommandPackData_create(scope, packer);
    return self;
}

ICommandHandle CommandPackDataParser_getCommand(CommandPackDataParserHandle self){
    return CommandPackData_getICommand(self->command);
}

void CommandPackDataParser_destroy(CommandPackDataParserHandle self){
    CommandPackData_destroy(self->command);

    free(self);
    self = NULL;
}
