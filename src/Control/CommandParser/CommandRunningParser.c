/*!****************************************************************************************************************************************
 * @file         CommandRunningParser.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Control/CommandParser/CommandRunningParser.h>
#include <Scope/Control/Command/CommandRunning.h>
#include <Scope/Control/IUnpacker.h>
#include <Scope/Core/ScopeTypes.h>
#include <Scope/Control/ParserDefinitions.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Name of the command */
static char* commandName = "cf_running";

/* Class data */
typedef struct __CommandRunningParserPrivateData{
    CommandRunningHandle command;
    IUnpackerHandle unpacker;

} CommandRunningParserPrivateData;

/******************************************************************************
 Public functions
******************************************************************************/
CommandRunningParserHandle CommandRunningParser_create(IScopeHandle scope, IUnpackerHandle unpacker){
    CommandRunningParserHandle self = malloc(sizeof(CommandRunningParserPrivateData));
    self->command = CommandRunning_create(scope);
    self->unpacker = unpacker;
    return self;
}

ICommandHandle CommandRunningParser_getCommand(CommandRunningParserHandle self){

    if(self->unpacker == NULL){
        return NULL;
    }

    const int numberOfFields = self->unpacker->getNumberOfFields(self->unpacker, (const char*) commandName);

    if(numberOfFields == -1){
        return NULL;
    }

    uint32_t channelIds[numberOfFields];
    CHANNEL_STATES newStates[numberOfFields];

    char nameOfField[MAX_FIELD_LENGTH];

    for(size_t i = 0; i < numberOfFields; ++i){

        bool foundField = self->unpacker->getNameOfField(self->unpacker, commandName, nameOfField, MAX_FIELD_LENGTH, i);

        if(foundField == true){
            char* endPtr;
            channelIds[i] = (uint32_t) strtoul(nameOfField, &endPtr, 10);

            if(*endPtr != '\0'){
                return NULL;
            }

            CommandFetchingInformation information = {.commandName = commandName, .fieldName = nameOfField,
                    .isInArray = false, .arrayIndex = 0};

            bool newState = self->unpacker->getBoolFromCommand(self->unpacker, &information);
            if(newState == true){
                newStates[i] = CHANNEL_RUNNING;
            }else{
                newStates[i] = CHANNEL_STOPPED;
            }
        }
    }

    CommandRunningConf conf = {.newStates = newStates, \
                          .changedChannels = channelIds, \
                          .numberOfChangedChannels = numberOfFields};

    CommandRunning_setAttributes(self->command, conf);

    return CommandRunning_getICommand(self->command);
}

char* CommandRunningParser_getName(void){
    return commandName;
}

void CommandRunningParser_destroy(CommandRunningParserHandle self){
    CommandRunning_destroy(self->command);

    free(self);
    self = NULL;
}
