/*!****************************************************************************************************************************************
 * @file         CommandTIncParser.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Command/CommandTIncParser.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandTIncParserPrivateData
{
  ICommandHandle command;
  IUnpackerHandle unpacker;
  char* commandName;

} CommandTIncParserPrivateData ;

/******************************************************************************
 Public functions
******************************************************************************/
CommandTIncParserHandle CommandTIncParser_create(ICommandHandle command, IUnpackerHandle unpacker){
  CommandTIncParserHandle self = malloc(sizeof(CommandTIncParserPrivateData));
  self->command = command;
  self->unpacker = unpacker;
  self->commandName = (char*) self->command->getCommandName(self->command);
  return self;
}

void CommandTIncParser_configure(CommandTIncParserHandle self){

  if(self->unpacker == NULL){
    return;
  }

  CommandFetchingInformation information = { .commandName = self->commandName, .fieldName = (char*) "",
                                             .isInArray = false, .arrayIndex = 0 };

  const int timeIncrement = self->unpacker->getIntFromCommand(self->unpacker, &information);

  self->command->setCommandAttribute(self->command, (void*) &timeIncrement);
}

void CommandTIncParser_destroy(CommandTIncParserHandle self){
  free(self);
  self = NULL;
}
