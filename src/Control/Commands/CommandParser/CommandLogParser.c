/*!****************************************************************************************************************************************
 * @file         CommandLogParser.c
 *
 * @copyright    Copyright (c) 2020 by Sourceengineers. All Rights Reserved.
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
