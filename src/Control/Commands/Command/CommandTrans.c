/*!****************************************************************************************************************************************
 * @file         CommandTrans.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Control/Commands/Command/CommandTrans.h>
#include <Scope/Control/ICommand.h>
#include <Scope/Core/IScope.h>

#include <stdlib.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandTransPrivateData{
    ICommand command;

    IScopeHandle scope;

} CommandTransPrivateData;

static void run(ICommandHandle command);

/******************************************************************************
 Private functions
******************************************************************************/
static void run(ICommandHandle command){
    CommandTransHandle self = (CommandTransHandle) command->handle;
    self->scope->transmit(self->scope);
}

/******************************************************************************
 Private functions
******************************************************************************/
CommandTransHandle CommandTrans_create(IScopeHandle scope){

    CommandTransHandle self = malloc(sizeof(CommandTransPrivateData));
    assert(self);

    self->scope = scope;

    self->command.handle = self;
    self->command.run = &run;

    return self;
}

ICommandHandle CommandTrans_getICommand(CommandTransHandle self){
    return &self->command;
}

void CommandTrans_destroy(CommandTransHandle self){
    free(self);
    self = NULL;
}
