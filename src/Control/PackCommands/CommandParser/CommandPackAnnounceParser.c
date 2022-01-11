/*!****************************************************************************************************************************************
 * @file         CommandPackAnnounceParser.c
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
