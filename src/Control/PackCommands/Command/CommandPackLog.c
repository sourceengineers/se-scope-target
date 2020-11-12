/*!****************************************************************************************************************************************
* @file         CommandPackLog.c
*
* @copyright    Copyright (c) 2020 by Sourceengineers. All Rights Reserved.
*
* @authors      Anselm Fuhrer anselm.fuhrer@sourceengineers.com
*
*****************************************************************************************************************************************/

#include "Scope/GeneralPurpose/DataTypes.h"

#include "Scope/Control/PackCommands/Command/CommandPackLog.h"
#include "Scope/Control/PackCommands/Command/CommandPackAnnounce.h"
#include "Scope/Control/IPacker.h"
#include "Scope/Control/ICommand.h"
#include "Scope/Core/ScopeTypes.h"
#include "Scope/Core/IScope.h"
#include "Scope/Version.h"

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandPackLogPrivateData{
    ICommand command;
    IPackerHandle packer;
    ScLogDataDef log;
} CommandPackLogPrivateData;

static void run(ICommandHandle command);

/******************************************************************************
 Private functions
******************************************************************************/
static void run(ICommandHandle command){
    CommandPackLogHandle self = (CommandPackLogHandle) command->handle;
    self->packer->addLog(self->packer, self->log);
}

/******************************************************************************
 Private functions
******************************************************************************/
CommandPackLogHandle CommandPackLog_create(ScLogDataDef log, IPackerHandle packer){
    CommandPackLogHandle self = malloc(sizeof(CommandPackLogPrivateData));
    assert(self);

    self->log = log;	//TODO where do i get this from?
    self->packer = packer;

    self->command.handle = self;
    self->command.run = &run;

    return self;
}

ICommandHandle CommandPackLog_getICommand(CommandPackLogHandle self){
    return &self->command;
}

void CommandPackLog_destroy(CommandPackLogHandle self){
    free(self);
    self = NULL;
}
