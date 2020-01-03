/*!****************************************************************************************************************************************
 * @file         CommandTriggerParser.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
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
