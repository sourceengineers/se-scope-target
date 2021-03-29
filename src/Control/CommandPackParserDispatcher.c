/*!****************************************************************************************************************************************
 * @file         CommandPackParserDispatcher.c
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

#include "Scope/Control/CommandPackParserDispatcher.h"
#include "Scope/Control/PackCommands/CommandParser/CommandPackAnnounceParser.h"
#include "Scope/Control/PackCommands/CommandParser/CommandPackDataParser.h"
#include "Scope/Control/ParserDefinitions.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandPackParserDispatcherPrivateData{
    CommandPackDataParserHandle commandPackDataParser;
    CommandPackAnnounceParserHandle commandPackAnnounceParser;
} CommandPackParserDispatcherPrivateData;


/******************************************************************************
 Public functions
******************************************************************************/
CommandPackParserDispatcherHandle
CommandPackParserDispatcher_create(IScopeHandle scope, AnnounceStorageHandle announceStorage, IPackerHandle packer){

    CommandPackParserDispatcherHandle self = malloc(sizeof(CommandPackParserDispatcherPrivateData));
    assert(self);

    self->commandPackAnnounceParser = CommandPackAnnounceParser_create(announceStorage, packer);
    self->commandPackDataParser = CommandPackDataParser_create(scope, packer);

    return self;
}

ICommandHandle CommandPackParserDispatcher_run(CommandPackParserDispatcherHandle self, MessageType type){

    if(type == SC_DATA){
        return CommandPackDataParser_getCommand(self->commandPackDataParser);
    }else if(type == SC_ANNOUNCE){
        return CommandPackAnnounceParser_getCommand(self->commandPackAnnounceParser);
    }

    return NULL;
}

void CommandPackParserDispatcher_destroy(CommandPackParserDispatcherHandle self){

    CommandPackDataParser_destroy(self->commandPackDataParser);
    CommandPackAnnounceParser_destroy(self->commandPackAnnounceParser);

    free(self);
    self = NULL;
}
