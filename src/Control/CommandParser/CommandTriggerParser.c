/*!****************************************************************************************************************************************
 * @file         CommandTriggerParser.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Control/CommandParser/CommandTriggerParser.h>
#include <string.h>
#include <Scope/Control/ParserDefinitions.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Name of the command */
static char* commandName = "cf_tgr";

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
CommandTriggerParserHandle CommandTriggerParser_create(IScopeHandle scope, IUnpackerHandle unpacker){
    CommandTriggerParserHandle self = malloc(sizeof(CommandTriggerParserPrivateData));
    self->command = CommandTrigger_create(scope);
    self->unpacker = unpacker;

    return self;
}

ICommandHandle CommandTriggerParser_getCommand(CommandTriggerParserHandle self){

    if(self->unpacker == NULL){
        return NULL;
    }

    const size_t maxStringSize = 20;

    CommandFetchingInformation information = {.commandName = commandName, .fieldName = KEYWORD_CF_TGR_CL_ID,
            .isInArray = false, .arrayIndex = 0};

    /* Get the correct data stream */
    const uint32_t channelId = self->unpacker->getIntFromCommand(self->unpacker, &information);

    /* Get the trigger level */
    information.fieldName = KEYWORD_CF_TGR_LEVEL;
    const float level = self->unpacker->getFloatFromCommand(self->unpacker, &information);

    /* Get the trigger mode */
    char triggerModeToParse[maxStringSize];
    information.fieldName = KEYWORD_CF_TGR_MODE;
    self->unpacker->getStringFromCommand(self->unpacker, &information, triggerModeToParse, maxStringSize);
    TRIGGER_MODE mode = parseStringToTriggerMode(triggerModeToParse);

    /* Get the trigger edge */
    char edgeToParse[maxStringSize];
    information.fieldName = KEYWORD_CF_TGR_EDGE;
    self->unpacker->getStringFromCommand(self->unpacker, &information, edgeToParse, maxStringSize);
    int edge = parseStringToEdge(edgeToParse);

    TriggerConfiguration conf = {.level = level, .mode = mode, .edge = edge, .channelId = channelId};

    CommandTrigger_setAttributes(self->command, conf);

    return CommandTrigger_getICommand(self->command);
}

char* CommandTriggerParser_getName(){
    return commandName;
}

void CommandTriggerParser_destroy(CommandTriggerParserHandle self){
    CommandTrigger_destroy(self->command);

    free(self);
    self = NULL;
}
