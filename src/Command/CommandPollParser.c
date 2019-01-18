/*!****************************************************************************************************************************************
 * @file         CommandPollParser.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Command/CommandPollParser.h>

/******************************************************************************
 Define private data
******************************************************************************/
//* Name of the command */
static char* commandName = "ev_poll";

/* Class data */
typedef struct __CommandPollParserPrivateData{
    CommandPollHandle command;
    IUnpackerHandle unpacker;

} CommandPollParserPrivateData;

/******************************************************************************
 Public functions
******************************************************************************/
CommandPollParserHandle CommandPollParser_create(IScopeHandle scope, IUnpackerHandle unpacker){
  CommandPollParserHandle self = malloc(sizeof(CommandPollParserPrivateData));
  self->command = CommandPoll_create(scope);
  self->unpacker = unpacker;
  return self;
}

ICommandHandle CommandPollParser_getCommand(CommandPollParserHandle self){

  if(self->unpacker == NULL){
    return NULL;
  }

  CommandFetchingInformation information = {.commandName = commandName, .fieldName = (char*) "",
          .isInArray = false, .arrayIndex = 0};

  const uint32_t timestamp = self->unpacker->getIntFromCommand(self->unpacker, &information);

  ICommandHandle command = CommandPoll_getICommand(self->command);
  command->setCommandAttribute(command, (void*) &timestamp);

  return command;
}

char* CommandPollParser_getName(){
  return commandName;
}

void CommandPollParser_destroy(CommandPollParserHandle self){
  CommandPoll_destroy(self->command);

  free(self);
  self = NULL;
}
