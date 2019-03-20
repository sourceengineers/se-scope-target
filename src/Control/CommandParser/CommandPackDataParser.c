/*!****************************************************************************************************************************************
 * @file         CommandPackDataParser.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Control/CommandParser/CommandPackDataParser.h>
#include <Scope/Control/Command/CommandPackData.h>

#include <stdlib.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Name of the command */
static char* commandName = "ev_pack_data";

/* Class data */
typedef struct __CommandPackDataParserPrivateData{
    CommandPackDataHandle command;

} CommandPackDataParserPrivateData;

/******************************************************************************
 Public functions
******************************************************************************/
CommandPackDataParserHandle CommandPackDataParser_create(IScopeHandle scope, IPackerHandle packer){
    CommandPackDataParserHandle self = malloc(sizeof(CommandPackDataParserPrivateData));
    self->command = CommandPackData_create(scope, packer);
    return self;
}

ICommandHandle CommandPackDataParser_getCommand(CommandPackDataParserHandle self){
    return CommandPackData_getICommand(self->command);
}

char* CommandPackDataParser_getName(void){
    return commandName;
}

void CommandPackDataParser_destroy(CommandPackDataParserHandle self){
    CommandPackData_destroy(self->command);

    free(self);
    self = NULL;
}
