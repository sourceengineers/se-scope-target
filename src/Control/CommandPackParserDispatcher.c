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
} CommandPackParserDispatcherPrivateData;


/******************************************************************************
 Public functions
******************************************************************************/
CommandPackParserDispatcherHandle
CommandPackParserDispatcher_create(IScopeHandle scope, IPackerHandle packer){

    CommandPackParserDispatcherHandle self = malloc(sizeof(CommandPackParserDispatcherPrivateData));

    self->commandPackAnnounceParser = CommandPackAnnounceParser_create(scope, packer);
    self->commandPackDataParser = CommandPackDataParser_create(scope, packer);
    
    return self;
}

ICommandHandle CommandPackParserDispatcher_run(CommandPackParserDispatcherHandle self, const char* command){

    if(strncmp(command, CommandPackDataParser_getName(), MAX_COMMAND_LENGTH) == 0){
        return CommandPackDataParser_getCommand(self->commandPackDataParser);

    }else if(strncmp(command, CommandPackAnnounceParser_getName(), MAX_COMMAND_LENGTH) == 0){
        return CommandPackAnnounceParser_getCommand(self->commandPackAnnounceParser);

    }

    return NULL;
}

void CommandPackParserDispatcher_destroy(CommandPackParserDispatcherHandle self){

    CommandPackDataParser_destroy(self->commandPackDataParser);
    CommandPackAnnounceParser_destroy(self->commandPackAnnounceParser);

    free(self);
    self = NULL;
}
