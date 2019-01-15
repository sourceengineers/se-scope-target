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
/* Class data */
typedef struct __CommandPollParserPrivateData
{
  ICommandHandle command;
  IUnpackerHandle unpacker;
  char* commandName;

} CommandPollParserPrivateData ;

/******************************************************************************
 Public functions
******************************************************************************/
CommandPollParserHandle CommandPollParser_create(ICommandHandle command, IUnpackerHandle unpacker){
  CommandPollParserHandle self = malloc(sizeof(CommandPollParserPrivateData));
  self->command = command;
  self->unpacker = unpacker;
  self->commandName = (char*) self->command->getCommandName(self->command);
  return self;
}

void CommandPollParser_configure(CommandPollParserHandle self){

  if(self->unpacker == NULL){
    return;
  }

  CommandFetchingInformation information = { .commandName = self->commandName, .fieldName = (char*) "",
                                             .isInArray = false, .arrayIndex = 0 };

  const uint32_t timestamp = self->unpacker->getIntFromCommand(self->unpacker, &information);

  self->command->setCommandAttribute(self->command, (void*) &timestamp);
}

void CommandPollParser_destroy(CommandPollParserHandle self){
  free(self);
  self = NULL;
}
