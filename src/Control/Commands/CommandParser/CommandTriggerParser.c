/*!****************************************************************************************************************************************
 * @file         CommandTriggerParser.c
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

#include "Scope/Control/Commands/CommandParser/CommandTriggerParser.h"
#include "Scope/Control/ParserDefinitions.h"
#include "Scope/Control/Commands/Command/CommandTrigger.h"
#include "Scope/Control/IUnpacker.h"
#include "Scope/Core/ScopeTypes.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandTriggerParserPrivateData{
    CommandTriggerHandle command;

    IUnpackerHandle unpacker;

} CommandTriggerParserPrivateData;

/******************************************************************************
 Private functions
******************************************************************************/

/******************************************************************************
 Public functions
******************************************************************************/
CommandTriggerParserHandle CommandTriggerParser_create(IScopeHandle scope, IUnpackerHandle unpacker,
        IObserverHandle observer){
    CommandTriggerParserHandle self = malloc(sizeof(CommandTriggerParserPrivateData));
    assert(self);
    self->command = CommandTrigger_create(scope, observer);
    self->unpacker = unpacker;

    return self;
}

ICommandHandle CommandTriggerParser_getCommand(CommandTriggerParserHandle self){

    if(self->unpacker == NULL){
        return NULL;
    }

    CfTriggerDef trigger = self->unpacker->cfTrigger_getTriggerConfig(self->unpacker);

    TriggerConfiguration conf = {
            .level = trigger.level,
            .mode = trigger.mode,
            .edge = trigger.edge,
            .channelId = trigger.channelId
    };

    CommandTrigger_setAttributes(self->command, conf);

    return CommandTrigger_getICommand(self->command);
}

void CommandTriggerParser_destroy(CommandTriggerParserHandle self){
    CommandTrigger_destroy(self->command);

    free(self);
    self = NULL;
}
