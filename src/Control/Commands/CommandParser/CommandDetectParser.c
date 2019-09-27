/*!****************************************************************************************************************************************
 * @file         CommandDetectParser.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Control/Commands/CommandParser/CommandDetectParser.h>
#include <Scope/Control/Commands/Command/CommandDetect.h>
#include <Scope/Control/IUnpacker.h>
#include <Scope/Control/ParserDefinitions.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/
//* Name of the command */
static char* commandName = "ev_detect";

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
CommandDetectParserHandle CommandDetectParser_create(IObserverHandle packOverser){
    CommandDetectParserHandle self = malloc(sizeof(CommandDetectParserPrivateData));
    assert(self);

    self->command = CommandDetect_create(packOverser);
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

char* CommandDetectParser_getName(void){
    return commandName;
}
