/*!****************************************************************************************************************************************
* @file         CommandPackDetect.c
*
* @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
*
* @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
*
*****************************************************************************************************************************************/

#include <Scope/Control/Command/CommandPackDetect.h>
#include <Scope/Control/IPacker.h>
#include <Scope/Control/Command/ICommand.h>
#include <Scope/Core/ScopeTypes.h>
#include <Scope/Core/IScope.h>
#include <Scope/GeneralPurpose/DataTypes.h>

#include <stdint.h>
#include <stdlib.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandPackDetectPrivateData{
    ICommand command;

    IPackerHandle packer;

} CommandPackDetectPrivateData;

static void run(ICommandHandle command);

/******************************************************************************
 Private functions
******************************************************************************/
static void run(ICommandHandle command){
    CommandPackDetectHandle self = (CommandPackDetectHandle) command->handle;

    if(self->packer == NULL){
        return;
    }

    self->packer->prepareDetect(self->packer);

}

/******************************************************************************
 Private functions
******************************************************************************/
CommandPackDetectHandle CommandPackDetect_create(IPackerHandle packer){

    CommandPackDetectHandle self = malloc(sizeof(CommandPackDetectPrivateData));
    self->packer = packer;

    self->command.handle = self;
    self->command.run = &run;

    return self;
}

ICommandHandle CommandPackDetect_getICommand(CommandPackDetectHandle self){
    return &self->command;
}

void CommandPackDetect_destroy(CommandPackDetectHandle self){
    free(self);
    self = NULL;
}
