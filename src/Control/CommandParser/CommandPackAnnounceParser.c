/*!****************************************************************************************************************************************
 * @file         CommandPackAnnounceParser.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Control/CommandParser/CommandPackAnnounceParser.h>
#include <Scope/Control/Command/CommandPackAnnounce.h>

#include <stdlib.h>
#include <Scope/Control/AnnounceStorage.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Name of the command */
static char* commandName = "ev_pack_announce";

/* Class data */
typedef struct __CommandPackAnnounceParserPrivateData{
    CommandPackAnnounceHandle command;

} CommandPackAnnounceParserPrivateData;

/******************************************************************************
 Public functions
******************************************************************************/
CommandPackAnnounceParserHandle CommandPackAnnounceParser_create(AnnounceStorageHandle addressStorage, IPackerHandle packer){
    CommandPackAnnounceParserHandle self = malloc(sizeof(CommandPackAnnounceParserPrivateData));
    assert(self);

    self->command = CommandPackAnnounce_create(addressStorage, packer);
    return self;
}

ICommandHandle CommandPackAnnounceParser_getCommand(CommandPackAnnounceParserHandle self){
    return CommandPackAnnounce_getICommand(self->command);
}

char* CommandPackAnnounceParser_getName(void){
    return commandName;
}

void CommandPackAnnounceParser_destroy(CommandPackAnnounceParserHandle self){
    CommandPackAnnounce_destroy(self->command);

    free(self);
    self = NULL;
}
