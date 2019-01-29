/*!****************************************************************************************************************************************
 * @file         CommandParserDispatcher.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Control/CommandParserDispatcher.h>
#include <string.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandParserDispatcherPrivateData{
    CommandAddrParserHandle commandAddrParser;
    CommandRunningParserHandle commandRunningParser;
    CommandTIncParserHandle commandTIncParser;
    CommandTriggerParserHandle commandTriggerParser;
    CommandPollParserHandle commandPollParser;
    CommandAnnounceParserHandle commandAnnounceParser;
    CommandTransParserHandle commandTransParser;
    CommandClearParserHandle commandClearParser;
} CommandParserDispatcherPrivateData;


/******************************************************************************
 Public functions
******************************************************************************/
CommandParserDispatcherHandle
CommandParserDispatcher_create(IScopeHandle scope, IPackerHandle packer, IUnpackerHandle unpacker){

    CommandParserDispatcherHandle self = malloc(sizeof(CommandParserDispatcherPrivateData));

    /* Initialize needed command parser
    * Not all commands need parser, which is why there are less parser as commands */
    self->commandAddrParser = CommandAddrParser_create(scope, unpacker);
    self->commandRunningParser = CommandRunningParser_create(scope, unpacker);
    self->commandTIncParser = CommandTIncParser_create(scope, unpacker);
    self->commandTriggerParser = CommandTriggerParser_create(scope, unpacker);
    self->commandPollParser = CommandPollParser_create(scope);
    self->commandAnnounceParser = CommandAnnounceParser_create(scope);
    self->commandTransParser = CommandTransParser_create(scope);
    self->commandClearParser = CommandClearParser_create(scope);

    return self;
}

ICommandHandle CommandParserDispatcher_run(CommandParserDispatcherHandle self, const char* command){

    if(strncmp(command, CommandRunningParser_getName(), MAX_COMMAND_LENGTH) == 0){
        return CommandRunningParser_getCommand(self->commandRunningParser);

    }else if(strncmp(command, CommandPollParser_getName(), MAX_COMMAND_LENGTH) == 0){
        return CommandPollParser_getCommand(self->commandPollParser);

    }else if(strncmp(command, CommandAddrParser_getName(), MAX_COMMAND_LENGTH) == 0){
        return CommandAddrParser_getCommand(self->commandAddrParser);

    }else if(strncmp(command, CommandTIncParser_getName(), MAX_COMMAND_LENGTH) == 0){
        return CommandTIncParser_getCommand(self->commandTIncParser);

    }else if(strncmp(command, CommandTransParser_getName(), MAX_COMMAND_LENGTH) == 0){
        return CommandTransParser_getCommand(self->commandTransParser);

    }else if(strncmp(command, CommandTriggerParser_getName(), MAX_COMMAND_LENGTH) == 0){
        return CommandTriggerParser_getCommand(self->commandTriggerParser);

    }else if(strncmp(command, CommandAnnounceParser_getName(), MAX_COMMAND_LENGTH) == 0){
        return CommandAnnounceParser_getCommand(self->commandAnnounceParser);

    }else if(strncmp(command, CommandClearParser_getName(), MAX_COMMAND_LENGTH) == 0){
        return CommandClearParser_getCommand(self->commandClearParser);

    }

    return NULL;
}

void CommandParserDispatcher_destroy(CommandParserDispatcherHandle self){

    CommandRunningParser_destroy(self->commandRunningParser);
    CommandAddrParser_destroy(self->commandAddrParser);
    CommandTIncParser_destroy(self->commandTIncParser);
    CommandTriggerParser_destroy(self->commandTriggerParser);
    CommandPollParser_destroy(self->commandPollParser);
    CommandClearParser_destroy(self->commandClearParser);
    CommandAnnounceParser_destroy(self->commandAnnounceParser);
    CommandTransParser_destroy(self->commandTransParser);

    free(self);
    self = NULL;
}
