/*!****************************************************************************************************************************************
 * @file         CommandAnnounce.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Command/CommandAnnounce.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandAnnouncePrivateData{
    ICommand command;
    IScopeHandle scope;

} CommandAnnouncePrivateData;

/******************************************************************************
 Private functions
******************************************************************************/
static void run(ICommandHandle command){
  CommandAnnounceHandle self = (CommandAnnounceHandle) command->implementer;
  self->scope->announce(self->scope);
}

static void setCommandAttribute(ICommandHandle command, void* attr){
  return;
}

/******************************************************************************
 Private functions
******************************************************************************/
CommandAnnounceHandle CommandAnnounce_create(IScopeHandle scope){

  CommandAnnounceHandle self = malloc(sizeof(CommandAnnouncePrivateData));
  self->scope = scope;

  self->command.implementer = self;
  self->command.run = &run;
  self->command.setCommandAttribute = &setCommandAttribute;

  return self;
}

ICommandHandle CommandAnnounce_getICommand(CommandAnnounceHandle self){
  return &self->command;
}

void CommandAnnounce_destroy(CommandAnnounceHandle self){
  free(self);
  self = NULL;
}
