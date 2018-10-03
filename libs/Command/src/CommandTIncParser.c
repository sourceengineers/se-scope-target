/*!****************************************************************************************************************************************
 * @file         CommandTIncParser.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Command/CommandTIncParser.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandTIncParserPrivateData
{
  ICommandHandle iCommand;
  IUnpackerHandle iUnpacker;
  char* commandName;

} CommandTIncParserPrivateData ;

/******************************************************************************
 Public functions
******************************************************************************/
CommandTIncParserHandle CommandTIncParser_create(ICommandHandle iCommand, IUnpackerHandle iUnpacker){
  CommandTIncParserHandle self = malloc(sizeof(CommandTIncParserPrivateData));
  self->iCommand = iCommand;
  self->iUnpacker = iUnpacker;
  self->commandName = (char*) self->iCommand->getCommandName(self->iCommand);
  return self;
}

void CommandTIncParser_configure(CommandTIncParserHandle self){

  const int timeIncrement = self->iUnpacker->getIntFromCommand(self->iUnpacker, (const char*) self->commandName,
                                                        (const char*) "");

  self->iCommand->setCommandAttribute(self->iCommand, (void*) &timeIncrement);
}

void CommandTIncParser_destroy(CommandTIncParserHandle self){
  free(self);
  self = NULL;
}
