/*!****************************************************************************************************************************************
 * @file         CommandPackLogParser.c
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
 * @authors     Anselm Fuhrer anselm.fuhrer@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include "Scope/Control/PackCommands/CommandParser/CommandPackLogParser.h"
#include "Scope/Control/PackCommands/Command/CommandPackLog.h"

#include <stdlib.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandPackLogParserPrivateData{
	CommandPackLogHandle command;
} CommandPackLogParserPrivateData;

/******************************************************************************
 Public functions
******************************************************************************/

CommandPackLogParserHandle CommandPackLogParser_create(IByteStreamHandle logStream, IPackerHandle packer){
    CommandPackLogParserHandle self = malloc(sizeof(CommandPackLogParserPrivateData));
    assert(self);
    self->command = CommandPackLog_create(logStream, packer);
    return self;
}

ICommandHandle CommandPackLogParser_getCommand(CommandPackLogParserHandle self){
    return CommandPackLog_getICommand(self->command);
}

void CommandPackLogParser_destroy(CommandPackLogParserHandle self){
    CommandPackLog_destroy(self->command);
    free(self);
    self = NULL;
}
