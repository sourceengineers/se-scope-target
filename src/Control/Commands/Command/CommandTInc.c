/*!****************************************************************************************************************************************
 * @file         CommandTInc.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include "Scope/Control/Commands/Command/CommandTInc.h"
#include "Scope/Control/ICommand.h"
#include "Scope/Core/IScope.h"

#include <stdlib.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/

/* Class data */
typedef struct __CommandTIncPrivateData{
    ICommand command;

    IScopeHandle scope;

    uint32_t timeIncrement;
} CommandTIncPrivateData;

static void run(ICommandHandle command);

/******************************************************************************
 Private functions
******************************************************************************/
static void run(ICommandHandle command){
    CommandTIncHandle self = (CommandTIncHandle) command->handle;
    self->scope->setTimeIncrement(self->scope, self->timeIncrement);
}

/******************************************************************************
 Private functions
******************************************************************************/
CommandTIncHandle CommandTInc_create(IScopeHandle scope){

    CommandTIncHandle self = malloc(sizeof(CommandTIncPrivateData));
    assert(self);

    self->scope = scope;
    self->timeIncrement = 0;

    self->command.handle = self;
    self->command.run = &run;

    return self;
}

ICommandHandle CommandTInc_getICommand(CommandTIncHandle self){
    return &self->command;
}

void CommandTInc_setAttributes(CommandTIncHandle self, uint32_t timeIncrement){
    self->timeIncrement = timeIncrement;
}

void CommandTInc_destroy(CommandTIncHandle self){
    free(self);
    self = NULL;
}
