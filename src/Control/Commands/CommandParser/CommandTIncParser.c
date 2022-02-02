/*!****************************************************************************************************************************************
 * @file         CommandTIncParser.c
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
 * @authors      Samuel Schuepbach <samuel.schuepbach@sourceengineers.com>
 *
 *****************************************************************************************************************************************/

#include "Scope/Control/Commands/CommandParser/CommandTIncParser.h"
#include "Scope/Control/Commands/Command/CommandTInc.h"
#include "Scope/Control/IUnpacker.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandTIncParserPrivateData{
    CommandTIncHandle command;

    IUnpackerHandle unpacker;

} CommandTIncParserPrivateData;

/******************************************************************************
 Public functions
******************************************************************************/
CommandTIncParserHandle CommandTIncParser_create(IScopeHandle scope, IUnpackerHandle unpacker,
        IObserverHandle observer){
    CommandTIncParserHandle self = malloc(sizeof(CommandTIncParserPrivateData));
    assert(self);

    self->command = CommandTInc_create(scope, observer);
    self->unpacker = unpacker;
    return self;
}

ICommandHandle CommandTIncParser_getCommand(CommandTIncParserHandle self){

    if(self->unpacker == NULL){
        return NULL;
    }

    const uint32_t timeIncrement = self->unpacker->cfTInc_getInc(self->unpacker);
    CommandTInc_setAttributes(self->command, timeIncrement);
    return CommandTInc_getICommand(self->command);
}

void CommandTIncParser_destroy(CommandTIncParserHandle self){
    CommandTInc_destroy(self->command);

    free(self);
    self = NULL;
}
