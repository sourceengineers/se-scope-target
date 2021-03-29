/*!****************************************************************************************************************************************
 * @file         CommandPackAnnounceParser.c
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

#include "Scope/Control/AnnounceStorage.h"

#include "Scope/Control/PackCommands/CommandParser/CommandPackAnnounceParser.h"
#include "Scope/Control/PackCommands/Command/CommandPackAnnounce.h"

#include <stdlib.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandPackAnnounceParserPrivateData{
    CommandPackAnnounceHandle command;

} CommandPackAnnounceParserPrivateData;

/******************************************************************************
 Public functions
******************************************************************************/
CommandPackAnnounceParserHandle CommandPackAnnounceParser_create(AnnounceStorageHandle announceStorage, IPackerHandle packer){
    CommandPackAnnounceParserHandle self = malloc(sizeof(CommandPackAnnounceParserPrivateData));
    assert(self);

    self->command = CommandPackAnnounce_create(announceStorage, packer);
    return self;
}

ICommandHandle CommandPackAnnounceParser_getCommand(CommandPackAnnounceParserHandle self){
    return CommandPackAnnounce_getICommand(self->command);
}

void CommandPackAnnounceParser_destroy(CommandPackAnnounceParserHandle self){
    CommandPackAnnounce_destroy(self->command);

    free(self);
    self = NULL;
}
