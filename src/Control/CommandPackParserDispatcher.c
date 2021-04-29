/*!****************************************************************************************************************************************
 * @file         CommandPackParserDispatcher.c
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
 *               is not valid. In this case the commercial license received with the purchase
 *               is applied (See SeScopeLicense.pdf).
 *               Please contact us at scope@sourceengineers.com for a commercial license.
 * }
 *
 * @authors      Samuel Schuepbach <samuel.schuepbach@sourceengineers.com>
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
