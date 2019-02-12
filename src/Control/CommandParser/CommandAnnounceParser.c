/*!****************************************************************************************************************************************
 * @file         CommandAnnounceParser.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Control/CommandParser/CommandAnnounceParser.h>

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
CommandAnnounceParserHandle CommandAnnounceParser_create(IScopeHandle scope){
    CommandAnnounceParserHandle self = malloc(sizeof(CommandAnnounceParserPrivateData));
    self->command = CommandAnnounce_create(scope);
    return self;
}

ICommandHandle CommandAnnounceParser_getCommand(CommandAnnounceParserHandle self){
    return CommandAnnounce_getICommand(self->command);
}

char* CommandAnnounceParser_getName(){
    return commandName;
}

void CommandAnnounceParser_destroy(CommandAnnounceParserHandle self){
    CommandAnnounce_destroy(self->command);

    free(self);
    self = NULL;
}
