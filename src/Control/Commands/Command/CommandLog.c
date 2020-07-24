/*!****************************************************************************************************************************************
 * @file         CommandLog.c
 *
 * @copyright    Copyright (c) 2020 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Anselm Fuhrer anselm.fuhrer@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include "Scope/Control/Commands/Command/CommandLog.h"
#include "Scope/Control/ICommand.h"
#include "Scope/Core/IScope.h"

#include <stdlib.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandLogPrivateData{
    ICommand command;

    IObserverHandle packObserver;

} CommandLogPrivateData;

static void run(ICommandHandle command);

/******************************************************************************
 Private functions
******************************************************************************/
static void run(ICommandHandle command){
    CommandLogHandle self = (CommandLogHandle) command->handle;
    MessageType packType = SC_LOG;
    self->packObserver->update(self->packObserver, &packType);
}

/******************************************************************************
 Private functions
******************************************************************************/
CommandLogHandle CommandLog_create(IObserverHandle packObserver){

	CommandLogHandle self = malloc(sizeof(CommandLogPrivateData));
    assert(self);
    self->packObserver = packObserver;

    self->command.handle = self;
    self->command.run = &run;

    return self;
}

ICommandHandle CommandLog_getICommand(CommandLogHandle self){
    return &self->command;
}

void CommandLog_destroy(CommandLogHandle self){
    free(self);
    self = NULL;
}
