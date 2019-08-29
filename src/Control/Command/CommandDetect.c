/*!****************************************************************************************************************************************
 * @file         CommandDetect.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Control/Command/CommandDetect.h>
#include <Scope/Control/Command/ICommand.h>
#include <Scope/Core/IScope.h>

#include <stdlib.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandDetectPrivateData{
    ICommand command;

    IObserverHandle packObserver;

} CommandDetectPrivateData;

static void run(ICommandHandle command);

/******************************************************************************
 Private functions
******************************************************************************/
static void run(ICommandHandle command){
    CommandDetectHandle self = (CommandDetectHandle) command->handle;
    PACK_TYPES packType = PACK_DETECT;
    self->packObserver->update(self->packObserver, &packType);
}

/******************************************************************************
 Private functions
******************************************************************************/
CommandDetectHandle CommandDetect_create(IObserverHandle packObserver){

    CommandDetectHandle self = malloc(sizeof(CommandDetectPrivateData));
    self->packObserver = packObserver;

    self->command.handle = self;
    self->command.run = &run;

    return self;
}

ICommandHandle CommandDetect_getICommand(CommandDetectHandle self){
    return &self->command;
}

void CommandDetect_destroy(CommandDetectHandle self){
    free(self);
    self = NULL;
}
