/*!****************************************************************************************************************************************
 * @file         CommandAnnounceParser.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include "Scope/Control/Commands/CommandParser/CommandAnnounceParser.h"
#include "Scope/Control/Commands/Command/CommandAnnounce.h"

#include <stdlib.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Name of the command */
static char* commandName = "ev_announce";

/* Class data */
typedef struct __CommandAnnounceParserPrivateData{
    CommandAnnounceHandle command;

} CommandAnnounceParserPrivateData;

/******************************************************************************
 Public functions
******************************************************************************/
CommandAnnounceParserHandle CommandAnnounceParser_create(IObserverHandle packOverser){
    CommandAnnounceParserHandle self = malloc(sizeof(CommandAnnounceParserPrivateData));
    assert(self);

    self->command = CommandAnnounce_create(packOverser);
    return self;
}

ICommandHandle CommandAnnounceParser_getCommand(CommandAnnounceParserHandle self){
    return CommandAnnounce_getICommand(self->command);
}

char* CommandAnnounceParser_getName(void){
    return commandName;
}

void CommandAnnounceParser_destroy(CommandAnnounceParserHandle self){
    CommandAnnounce_destroy(self->command);

    free(self);
    self = NULL;
}
