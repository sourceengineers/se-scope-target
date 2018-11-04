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
  ICommandHandle iCommand;
  IUnpackerHandle iUnpacker;
  char* commandName;

} CommandPollParserPrivateData ;

/******************************************************************************
 Public functions
******************************************************************************/
CommandPollParserHandle CommandPollParser_create(ICommandHandle iCommand, IUnpackerHandle iUnpacker){
  CommandPollParserHandle self = malloc(sizeof(CommandPollParserPrivateData));
  self->iCommand = iCommand;
  self->iUnpacker = iUnpacker;
  self->commandName = (char*) self->iCommand->getCommandName(self->iCommand);
  return self;
}

void CommandPollParser_configure(CommandPollParserHandle self){

  if(self->iUnpacker == NULL){
    return;
  }

  CommandFetchingInformation information = { .commandName = self->commandName, .fieldName = (char*) "",
                                             .isInArray = false, .arrayIndex = 0 };

  const gemmi_uint timestamp = self->iUnpacker->getIntFromCommand(self->iUnpacker, &information);

  self->iCommand->setCommandAttribute(self->iCommand, (void*) &timestamp);
}

void CommandPollParser_destroy(CommandPollParserHandle self){
  free(self);
  self = NULL;
}
