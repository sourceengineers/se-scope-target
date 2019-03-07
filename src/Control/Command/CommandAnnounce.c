/*!****************************************************************************************************************************************
 * @file         CommandAnnounce.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Control/Command/CommandAnnounce.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandAnnouncePrivateData{
    ICommand command;

    IScopeHandle scope;

} CommandAnnouncePrivateData;

static void run(ICommandHandle command);

/******************************************************************************
 Private functions
******************************************************************************/
static void run(ICommandHandle command){
    CommandAnnounceHandle self = (CommandAnnounceHandle) command->handle;
    self->scope->announce(self->scope);
}

/******************************************************************************
 Private functions
******************************************************************************/
CommandAnnounceHandle CommandAnnounce_create(IScopeHandle scope){

    CommandAnnounceHandle self = malloc(sizeof(CommandAnnouncePrivateData));
    self->scope = scope;

    self->command.handle = self;
    self->command.run = &run;

    return self;
}

ICommandHandle CommandAnnounce_getICommand(CommandAnnounceHandle self){
    return &self->command;
}

void CommandAnnounce_destroy(CommandAnnounceHandle self){
    free(self);
    self = NULL;
}
