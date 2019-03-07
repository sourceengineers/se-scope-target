/*!****************************************************************************************************************************************
 * @file         CommandTrigger.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Control/Command/CommandTrigger.h>
#include <Scope/Core/IScope.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandTriggerPrivateData{
    ICommand command;

    IScopeHandle scope;
    TriggerConfiguration config;

} CommandTriggerPrivateData;

static void run(ICommandHandle command);

/******************************************************************************
 Private functions
******************************************************************************/
static void run(ICommandHandle command){
    CommandTriggerHandle self = (CommandTriggerHandle) command->handle;

    self->scope->configureTrigger(self->scope, self->config);
}

/******************************************************************************
Public functions
******************************************************************************/
CommandTriggerHandle CommandTrigger_create(IScopeHandle scope){

    CommandTriggerHandle self = malloc(sizeof(CommandTriggerPrivateData));

    self->command.handle = self;
    self->scope = scope;
    self->command.run = &run;

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
