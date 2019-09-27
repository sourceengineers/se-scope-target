/*!****************************************************************************************************************************************
 * @file         CommandTransParser.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Control/Commands/CommandParser/CommandTransParser.h>
#include <Scope/Control/Commands/Command/CommandTrans.h>

#include <stdlib.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Name of the command */
static char* commandName = "ev_trans";

/* Class data */
typedef struct __CommandTransParserPrivateData{
    CommandTransHandle command;

} CommandTransParserPrivateData;

/******************************************************************************
 Public functions
******************************************************************************/
CommandTransParserHandle CommandTransParser_create(IScopeHandle scope){
    CommandTransParserHandle self = malloc(sizeof(CommandTransParserPrivateData));
    assert(self);

    self->command = CommandTrans_create(scope);
    return self;
}

ICommandHandle CommandTransParser_getCommand(CommandTransParserHandle self){
    return CommandTrans_getICommand(self->command);
}

char* CommandTransParser_getName(void){
    return commandName;
}

void CommandTransParser_destroy(CommandTransParserHandle self){
    CommandTrans_destroy(self->command);

    free(self);
    self = NULL;
}
