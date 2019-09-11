/*!****************************************************************************************************************************************
 * @file         CommandPackParserDispatcher.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Control/CommandPackParserDispatcher.h>
#include <Scope/Control/CommandParser/CommandPackAnnounceParser.h>
#include <Scope/Control/CommandParser/CommandPackDataParser.h>
#include <Scope/Control/CommandParser/CommandPackDetectParser.h>
#include <Scope/Core/ScopeTypes.h>

#include <stdlib.h>
#include <string.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandPackParserDispatcherPrivateData{
    CommandPackDataParserHandle commandPackDataParser;
    CommandPackAnnounceParserHandle commandPackAnnounceParser;
    CommandPackDetectParserHandle commandPackDetectParser;
} CommandPackParserDispatcherPrivateData;


/******************************************************************************
 Public functions
******************************************************************************/
CommandPackParserDispatcherHandle
CommandPackParserDispatcher_create(IScopeHandle scope, AnnounceStorageHandle addressStorage, IPackerHandle packer){

    CommandPackParserDispatcherHandle self = malloc(sizeof(CommandPackParserDispatcherPrivateData));

    self->commandPackAnnounceParser = CommandPackAnnounceParser_create(addressStorage, packer);
    self->commandPackDataParser = CommandPackDataParser_create(scope, packer);
    self->commandPackDetectParser = CommandPackDetectParser_create(packer);

    return self;
}

ICommandHandle CommandPackParserDispatcher_run(CommandPackParserDispatcherHandle self, const char* command){

    if(strncmp(command, CommandPackDataParser_getName(), MAX_COMMAND_LENGTH) == 0){
        return CommandPackDataParser_getCommand(self->commandPackDataParser);

    }else if(strncmp(command, CommandPackAnnounceParser_getName(), MAX_COMMAND_LENGTH) == 0){
        return CommandPackAnnounceParser_getCommand(self->commandPackAnnounceParser);

    }else if(strncmp(command, CommandPackDetectParser_getName(), MAX_COMMAND_LENGTH) == 0){
        return CommandPackDetectParser_getCommand(self->commandPackDetectParser);

    }

    return NULL;
}

void CommandPackParserDispatcher_destroy(CommandPackParserDispatcherHandle self){

    CommandPackDataParser_destroy(self->commandPackDataParser);
    CommandPackAnnounceParser_destroy(self->commandPackAnnounceParser);
    CommandPackDetectParser_destroy(self->commandPackDetectParser);

    free(self);
    self = NULL;
}
