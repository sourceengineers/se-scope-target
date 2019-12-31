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

/* Parses the given string into the correct trigger mode. Default is continious */
static TRIGGER_MODE parseStringToTriggerMode(const char* triggerModeString);

/* Parses the given string into a trigger edge. Default is rising edge */
static int parseStringToEdge(const char* edgeToParse);

/******************************************************************************
 Private functions
******************************************************************************/
static TRIGGER_MODE parseStringToTriggerMode(const char* triggerModeString){

    if(strncmp(triggerModeString, KEYWORD_CF_TGR_MODE_CONTINOUS, strlen(KEYWORD_CF_TGR_MODE_CONTINOUS)) == 0){
        return TRIGGER_CONTINUOUS;
    }else if(strncmp(triggerModeString, KEYWORD_CF_TGR_MODE_NORMAL, strlen(KEYWORD_CF_TGR_MODE_NORMAL)) == 0){
        return TRIGGER_NORMAL;
    }else if(strncmp(triggerModeString, KEYWORD_CF_TGR_MODE_ONESHOT, strlen(KEYWORD_CF_TGR_MODE_ONESHOT)) == 0){
        return TRIGGER_ONESHOT;
    }

    return TRIGGER_CONTINUOUS;
}

static int parseStringToEdge(const char* edgeToParse){

    if(strncmp(edgeToParse, KEYWORD_CF_TGR_EDGE_RISING, strlen(KEYWORD_CF_TGR_EDGE_RISING)) == 0){
        return TRIGGER_EDGE_POSITIVE;
    }else if(strncmp(edgeToParse, KEYWORD_CF_TGR_EDGE_FALLING, strlen(KEYWORD_CF_TGR_EDGE_FALLING)) == 0){
        return TRIGGER_EDGE_NEGATIVE;
    }

    return TRIGGER_EDGE_POSITIVE;
}

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
