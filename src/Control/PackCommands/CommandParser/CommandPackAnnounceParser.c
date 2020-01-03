/*!****************************************************************************************************************************************
 * @file         CommandPackAnnounceParser.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include "Scope/Control/AnnounceStorage.h"

#include "Scope/Control/PackCommands/CommandParser/CommandPackAnnounceParser.h"
#include "Scope/Control/PackCommands/Command/CommandPackAnnounce.h"

#include <stdlib.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandPackAnnounceParserPrivateData{
    CommandPackAnnounceHandle command;

} CommandPackAnnounceParserPrivateData;

/******************************************************************************
 Public functions
******************************************************************************/
CommandPackAnnounceParserHandle CommandPackAnnounceParser_create(AnnounceStorageHandle announceStorage, IPackerHandle packer){
    CommandPackAnnounceParserHandle self = malloc(sizeof(CommandPackAnnounceParserPrivateData));
    assert(self);

    self->command = CommandPackAnnounce_create(announceStorage, packer);
    return self;
}

ICommandHandle CommandPackAnnounceParser_getCommand(CommandPackAnnounceParserHandle self){
    return CommandPackAnnounce_getICommand(self->command);
}

void CommandPackAnnounceParser_destroy(CommandPackAnnounceParserHandle self){
    CommandPackAnnounce_destroy(self->command);

    free(self);
    self = NULL;
}
