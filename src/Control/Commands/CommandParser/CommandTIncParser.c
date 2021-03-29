/*!****************************************************************************************************************************************
 * @file         CommandTIncParser.c
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
