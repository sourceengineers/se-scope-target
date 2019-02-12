/*!****************************************************************************************************************************************
 * @file         CommandTIncParser.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Control/CommandParser/CommandTIncParser.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Name of the command */
static char* commandName = "cf_t_inc";

/* Class data */
typedef struct __CommandTIncParserPrivateData{
    CommandTIncHandle command;

    IUnpackerHandle unpacker;

} CommandTIncParserPrivateData;

/******************************************************************************
 Public functions
******************************************************************************/
CommandTIncParserHandle CommandTIncParser_create(IScopeHandle scope, IUnpackerHandle unpacker){
    CommandTIncParserHandle self = malloc(sizeof(CommandTIncParserPrivateData));
    self->command = CommandTInc_create(scope);
    self->unpacker = unpacker;
    return self;
}

ICommandHandle CommandTIncParser_getCommand(CommandTIncParserHandle self){

    if(self->unpacker == NULL){
        return NULL;
    }

    CommandFetchingInformation information = {.commandName = commandName, .fieldName = (char*) "",
            .isInArray = false, .arrayIndex = 0};

    const uint32_t timeIncrement = self->unpacker->getIntFromCommand(self->unpacker, &information);

    CommandTInc_setAttributes(self->command, timeIncrement);

    return CommandTInc_getICommand(self->command);
}

char* CommandTIncParser_getName(){
    return commandName;
}

void CommandTIncParser_destroy(CommandTIncParserHandle self){
    CommandTInc_destroy(self->command);

    free(self);
    self = NULL;
}
