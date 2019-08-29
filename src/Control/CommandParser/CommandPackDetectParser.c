/*!****************************************************************************************************************************************
 * @file         CommandPackDetectParser.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Control/CommandParser/CommandPackDetectParser.h>
#include <Scope/Control/Command/CommandPackDetect.h>

#include <stdlib.h>
#include <Scope/Control/AddressStorage.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Name of the command */
static char* commandName = "ev_pack_detect";

/* Class data */
typedef struct __CommandPackDetectParserPrivateData{
    CommandPackDetectHandle command;

} CommandPackDetectParserPrivateData;

/******************************************************************************
 Public functions
******************************************************************************/
CommandPackDetectParserHandle CommandPackDetectParser_create(IPackerHandle packer){
    CommandPackDetectParserHandle self = malloc(sizeof(CommandPackDetectParserPrivateData));
    self->command = CommandPackDetect_create(packer);
    return self;
}

ICommandHandle CommandPackDetectParser_getCommand(CommandPackDetectParserHandle self){
    return CommandPackDetect_getICommand(self->command);
}

char* CommandPackDetectParser_getName(void){
    return commandName;
}

void CommandPackDetectParser_destroy(CommandPackDetectParserHandle self){
    CommandPackDetect_destroy(self->command);

    free(self);
    self = NULL;
}