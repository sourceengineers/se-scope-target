/*!****************************************************************************************************************************************
 * @file         CommandClearParser.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Control/CommandParser/CommandClearParser.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Name of the command */
static char* commandName = "ev_clear";

/* Class data */
typedef struct __CommandClearParserPrivateData{
    CommandClearHandle command;

} CommandClearParserPrivateData;

/******************************************************************************
 Public functions
******************************************************************************/
CommandClearParserHandle CommandClearParser_create(IScopeHandle scope){
  CommandClearParserHandle self = malloc(sizeof(CommandClearParserPrivateData));
  self->command = CommandClear_create(scope);
  return self;
}

ICommandHandle CommandClearParser_getCommand(CommandClearParserHandle self){
  return CommandClear_getICommand(self->command);
}

char* CommandClearParser_getName(){
  return commandName;
}

void CommandClearParser_destroy(CommandClearParserHandle self){
  CommandClear_destroy(self->command);

  free(self);
  self = NULL;
}
