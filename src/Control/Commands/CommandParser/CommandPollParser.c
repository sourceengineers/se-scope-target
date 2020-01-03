/*!****************************************************************************************************************************************
 * @file         CommandPollParser.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include "Scope/Control/Commands/CommandParser/CommandPollParser.h"
#include "Scope/Control/Commands/Command/CommandPoll.h"

#include <stdlib.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandPollParserPrivateData{
    CommandPollHandle command;

} CommandPollParserPrivateData;

/******************************************************************************
 Public functions
******************************************************************************/
CommandPollParserHandle CommandPollParser_create(IScopeHandle scope, IObserverHandle observer){
    CommandPollParserHandle self = malloc(sizeof(CommandPollParserPrivateData));
    assert(self);

    self->command = CommandPoll_create(scope, observer);
    return self;
}

ICommandHandle CommandPollParser_getCommand(CommandPollParserHandle self){

    return CommandPoll_getICommand(self->command);
}

void CommandPollParser_destroy(CommandPollParserHandle self){
    CommandPoll_destroy(self->command);

    free(self);
    self = NULL;
}
