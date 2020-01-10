/*!****************************************************************************************************************************************
 * @file         CommandTrigger.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include "Scope/Control/Commands/Command/CommandTrigger.h"
#include "Scope/Control/ICommand.h"
#include "Scope/Core/IScope.h"
#include "Scope/Core/ScopeTypes.h"

#include <stdlib.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandTriggerPrivateData{
    ICommand command;

    IScopeHandle scope;
    TriggerConfiguration config;
    IObserverHandle observer;

} CommandTriggerPrivateData;

static void run(ICommandHandle command);

/******************************************************************************
 Private functions
******************************************************************************/
static void run(ICommandHandle command){
    CommandTriggerHandle self = (CommandTriggerHandle) command->handle;

    self->scope->configureTrigger(self->scope, self->config);
    MessageType resp = SE_ACK;
    self->observer->update(self->observer, &resp);
}

/******************************************************************************
Public functions
******************************************************************************/
CommandTriggerHandle CommandTrigger_create(IScopeHandle scope, IObserverHandle observer){

    CommandTriggerHandle self = malloc(sizeof(CommandTriggerPrivateData));
    assert(self);

    self->command.handle = self;
    self->scope = scope;
    self->command.run = &run;
    self->observer = observer;

    return self;
}

ICommandHandle CommandTrigger_getICommand(CommandTriggerHandle self){
    return &self->command;
}

void CommandTrigger_setAttributes(CommandTriggerHandle self, TriggerConfiguration conf){

    self->config = conf;
}

void CommandTrigger_destroy(CommandTriggerHandle self){
    free(self);
    self = NULL;
}
