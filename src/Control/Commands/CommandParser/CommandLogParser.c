/*!****************************************************************************************************************************************
 * @file         CommandLogParser.c
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
 * @authors      Anselm Fuhrer anselm.fuhrer@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include "Scope/GeneralPurpose/DataTypes.h"

#include "Scope/Control/Commands/CommandParser/CommandLogParser.h"
#include "Scope/Control/Commands/Command/CommandLog.h"
#include "Scope/Control/IUnpacker.h"
#include "Scope/Core/ScopeTypes.h"
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
typedef struct __CommandLogParserPrivateData{
    CommandLogHandle command;

    IUnpackerHandle unpacker;

} CommandLogParserPrivateData;

/******************************************************************************
 Private functions
******************************************************************************/

/******************************************************************************
 Public functions
******************************************************************************/
CommandLogParserHandle
CommandLogParser_create(IObserverHandle observer){
    //TODO implement for LogParser
	CommandLogParserHandle self = malloc(sizeof(CommandLogParserPrivateData));

    assert(self);
    self->command = CommandLog_create(observer);
    return self;
}

//TODO implement
ICommandHandle CommandLogParser_getCommand(CommandLogParserHandle self){

    if(self->unpacker == NULL){
        return NULL;
    }
    return NULL;	//TODO
    //return CommandLog_getICommand(self->command);

}

void CommandLogParser_destroy(CommandLogParserHandle self){
	CommandLog_destroy(self->command);
    free(self);
    self = NULL;
}
