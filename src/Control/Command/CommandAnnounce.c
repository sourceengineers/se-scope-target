/*!****************************************************************************************************************************************
 * @file         CommandAnnounce.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Control/Command/CommandAnnounce.h>
#include <Scope/Control/Command/ICommand.h>
#include <Scope/Core/IScope.h>

#include <stdlib.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandAnnouncePrivateData{
    ICommand command;

    IObserverHandle packObserver;

} CommandAnnouncePrivateData;

static void run(ICommandHandle command);

/******************************************************************************
 Private functions
******************************************************************************/
static void run(ICommandHandle command){
    CommandAnnounceHandle self = (CommandAnnounceHandle) command->handle;
    PACK_TYPES packType = PACK_ANNOUNCE;
    self->packObserver->update(self->packObserver, &packType);
}

/******************************************************************************
 Private functions
******************************************************************************/
CommandAnnounceHandle CommandAnnounce_create(IObserverHandle packObserver){

    CommandAnnounceHandle self = malloc(sizeof(CommandAnnouncePrivateData));
    self->packObserver = packObserver;

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
