/*!****************************************************************************************************************************************
 * @file         CommandDetectParser.c
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

#include "Scope/Control/Commands/CommandParser/CommandDetectParser.h"
#include "Scope/Control/Commands/Command/CommandDetect.h"
#include "Scope/Control/IUnpacker.h"
#include "Scope/Control/ParserDefinitions.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandDetectParserPrivateData{
    CommandDetectHandle command;

} CommandDetectParserPrivateData;

/******************************************************************************
 Private functions
******************************************************************************/

/******************************************************************************
 Public functions
******************************************************************************/
CommandDetectParserHandle CommandDetectParser_create(IObserverHandle observer){
    CommandDetectParserHandle self = malloc(sizeof(CommandDetectParserPrivateData));
    assert(self);

    self->command = CommandDetect_create(observer);
    return self;
}

ICommandHandle CommandDetectParser_getCommand(CommandDetectParserHandle self){

    return CommandDetect_getICommand(self->command);
}

/* Deconstructor: Deletes the instance of the channel */
void CommandDetectParser_destroy(CommandDetectParserHandle self){
    CommandDetect_destroy(self->command);

    free(self);
    self = NULL;
}
