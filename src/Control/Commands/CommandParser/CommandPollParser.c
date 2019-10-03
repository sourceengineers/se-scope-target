/*!****************************************************************************************************************************************
 * @file         CommandPollParser.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Control/Commands/CommandParser/CommandPollParser.h>
#include <Scope/Control/Commands/Command/CommandPoll.h>

#include <stdlib.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/
//* Name of the command */
static char* commandName = "ev_poll";

/* Class data */
typedef struct __CommandPollParserPrivateData{
    CommandPollHandle command;

} CommandPollParserPrivateData;

/******************************************************************************
 Public functions
******************************************************************************/
CommandPollParserHandle CommandPollParser_create(IScopeHandle scope){
    CommandPollParserHandle self = malloc(sizeof(CommandPollParserPrivateData));
    assert(self);

    self->command = CommandPoll_create(scope);
    return self;
}

ICommandHandle CommandPollParser_getCommand(CommandPollParserHandle self){

    return CommandPoll_getICommand(self->command);
}

char* CommandPollParser_getName(void){
    return commandName;
}

void CommandPollParser_destroy(CommandPollParserHandle self){
    CommandPoll_destroy(self->command);

    free(self);
    self = NULL;
}