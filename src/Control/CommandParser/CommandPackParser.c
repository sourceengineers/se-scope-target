/*!****************************************************************************************************************************************
 * @file         CommandPackParser.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Control/CommandParser/CommandPackParser.h>
#include <Scope/Control/IPacker.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Name of the command */
static char* commandName = "ev_pack";

/* Class data */
typedef struct __CommandPackParserPrivateData{
    CommandPackHandle command;

} CommandPackParserPrivateData;

/******************************************************************************
 Public functions
******************************************************************************/
CommandPackParserHandle CommandPackParser_create(IScopeHandle scope, IPackerHandle packer){
    CommandPackParserHandle self = malloc(sizeof(CommandPackParserPrivateData));
    self->command = CommandPack_create(scope, packer);
    return self;
}

ICommandHandle CommandPackParser_getCommand(CommandPackParserHandle self){
    return CommandPack_getICommand(self->command);
}

char* CommandPackParser_getName(){
    return commandName;
}

void CommandPackParser_destroy(CommandPackParserHandle self){
    CommandPack_destroy(self->command);

    free(self);
    self = NULL;
}
