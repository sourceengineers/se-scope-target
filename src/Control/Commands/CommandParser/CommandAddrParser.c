/*!****************************************************************************************************************************************
 * @file         CommandAddrParser.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include "Scope/GeneralPurpose/DataTypes.h"

#include "Scope/Control/Commands/CommandParser/CommandAddrParser.h"
#include "Scope/Control/Commands/Command/CommandAddr.h"
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
//* Name of the command */
static char* commandName = "cf_addr";

/* Class data */
typedef struct __CommandAddrParserPrivateData{
    CommandAddrHandle command;

    IUnpackerHandle unpacker;

} CommandAddrParserPrivateData;

/* Takes a String as input and retrieves the matching data type defined in "DataTypes.h> */
static DATA_TYPES parseStringToDataType(const char* dataTypeName, size_t maxLength);

/******************************************************************************
 Private functions
******************************************************************************/
static DATA_TYPES parseStringToDataType(const char* dataTypeName, const size_t maxLength){

    if(strncmp(dataTypeName, SE_UINT8_NAME, maxLength) == 0){
        return SE_UINT8;
    }else if(strncmp(dataTypeName, SE_UINT16_NAME, maxLength) == 0){
        return SE_UINT16;
    }else if(strncmp(dataTypeName, SE_UINT32_NAME, maxLength) == 0){
        return SE_UINT32;
    }else if(strncmp(dataTypeName, SE_UINT64_NAME, maxLength) == 0){
        return SE_UINT64;
    }else if(strncmp(dataTypeName, SE_FLOAT_NAME, maxLength) == 0){
        return SE_FLOAT;
    }

    return SE_FLOAT;
}

/******************************************************************************
 Public functions
******************************************************************************/
CommandAddrParserHandle CommandAddrParser_create(IScopeHandle scope, IUnpackerHandle unpacker, IObserverHandle observer){
    CommandAddrParserHandle self = malloc(sizeof(CommandAddrParserPrivateData));
    assert(self);

    self->command = CommandAddr_create(scope, observer);
    self->unpacker = unpacker;
    return self;
}

ICommandHandle CommandAddrParser_getCommand(CommandAddrParserHandle self){

    if(self->unpacker == NULL){
        return NULL;
    }

    const int numberOfFields = self->unpacker->getNumberOfFields(self->unpacker, commandName);

    if(numberOfFields == -1){
        return NULL;
    }

    uint32_t channelIds[numberOfFields];
    void* newAddresses[numberOfFields];
    DATA_TYPES types[numberOfFields];

    char nameOfField[MAX_FIELD_LENGTH];

    for(uint32_t i = 0; i < numberOfFields; i++){

        const bool foundField = self->unpacker->getNameOfField(self->unpacker, commandName, nameOfField,
                                                               MAX_FIELD_LENGTH, i);

        /* Only start parsing if the field was found, and its not a data type field */
        if(foundField == true){
            char* endPtr;
            channelIds[i] = (uint32_t) strtoul(nameOfField, &endPtr, 10);

            if(*endPtr != '\0'){
                return NULL;
            }

            CommandFetchingInformation information = {.commandName = commandName, .fieldName = nameOfField,
                    .isInArray = true, .arrayIndex = 0};

            newAddresses[i] = (void*) (ADDRESS_DATA_TYPE) self->unpacker->getIntFromCommand(self->unpacker,
                                                                                            &information);


            char requestedDataType[20];
            information.arrayIndex = 1;
            self->unpacker->getStringFromCommand(self->unpacker, &information, requestedDataType, 20);
            types[i] = parseStringToDataType(requestedDataType, 20);
        }
    }

    CommandAddrConf conf = {.newAddresses = newAddresses, \
                          .changedChannels = channelIds, \
                          .types = types, \
                          .numberOfChangedChannels = numberOfFields};

    CommandAddr_setAttributes(self->command, conf);

    return CommandAddr_getICommand(self->command);
}

/* Deconstructor: Deletes the instance of the channel */
void CommandAddrParser_destroy(CommandAddrParserHandle self){
    CommandAddr_destroy(self->command);

    free(self);
    self = NULL;
}

char* CommandAddrParser_getName(void){
    return commandName;
}
