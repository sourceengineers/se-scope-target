/*!****************************************************************************************************************************************
* @file         CommandPackData.c
*
* @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
*
* @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
*
*****************************************************************************************************************************************/

#include "Scope/Control/PackCommands/Command/CommandPackData.h"
#include "Scope/Control/IPacker.h"
#include "Scope/Control/ICommand.h"
#include "Scope/Core/ScopeTypes.h"
#include "Scope/Core/IScope.h"
#include "Scope/GeneralPurpose/FloatRingBuffer.h"
#include "Scope/GeneralPurpose/IIntStream.h"
#include "Scope/Control/ParserDefinitions.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandPackDataPrivateData{
    ICommand command;

    IScopeHandle scope;
    IPackerHandle packer;

} CommandPackDataPrivateData;

static void run(ICommandHandle command);

static bool mapTriggerModeToString(TRIGGER_MODE mode, char* buffer, size_t maxBufferSize);

/******************************************************************************
 Private functions
******************************************************************************/
static bool mapTriggerModeToString(TRIGGER_MODE mode, char* buffer, size_t maxBufferSize){

    if(mode == TRIGGER_CONTINUOUS){
        strncpy(buffer, KEYWORD_CF_TGR_MODE_CONTINOUS, maxBufferSize);
        return true;
    } else if(mode == TRIGGER_NORMAL){
        strncpy(buffer, KEYWORD_CF_TGR_MODE_NORMAL, maxBufferSize);
        return true;
    } else if(mode == TRIGGER_ONESHOT){
        strncpy(buffer, KEYWORD_CF_TGR_MODE_ONESHOT, maxBufferSize);
        return true;
    }

    strncpy(buffer, "", 1);
    return false;
}

static void run(ICommandHandle command){
    CommandPackDataHandle self = (CommandPackDataHandle) command->handle;

    bool anyChannelIsReady = false;

    for(uint32_t i = 0; i < self->scope->getAmountOfChannels(self->scope); ++i){
        if(self->scope->channelHasToBePacked(self->scope, i) == true){
            FloatRingBufferHandle buffer = self->scope->getChannelBuffer(self->scope, i);
            self->packer->addChannel(self->packer, buffer, i);
              anyChannelIsReady = true;
        }
    }

    if(anyChannelIsReady == false){
        return;
    }

    TriggeredValues triggeredValues = self->scope->getTriggerData(self->scope);

//    char triggerMode[KEYWORD_TGR_MODE_MAX_LENGTH];
//    mapTriggerModeToString(triggeredValues.mode, triggerMode, KEYWORD_TGR_MODE_MAX_LENGTH);

    self->packer->addTrigger(self->packer, triggeredValues.isTriggered, triggeredValues.channelId, \
                                   triggeredValues.triggerTimestamp, triggeredValues.mode);

    IIntStreamHandle scopeTimestamp = self->scope->getTimestamp(self->scope);
    self->packer->addTimestamp(self->packer, scopeTimestamp);

    const uint32_t timeIncrement = self->scope->getTimeIncrement(self->scope);
    self->packer->addTimeIncrement(self->packer, timeIncrement);
}

/******************************************************************************
 Private functions
******************************************************************************/
CommandPackDataHandle CommandPackData_create(IScopeHandle scope, IPackerHandle packer){

    CommandPackDataHandle self = malloc(sizeof(CommandPackDataPrivateData));
    assert(self);

    self->scope = scope;
    self->packer = packer;

    self->command.handle = self;
    self->command.run = &run;

    return self;
}

ICommandHandle CommandPackData_getICommand(CommandPackDataHandle self){
    return &self->command;
}

void CommandPackData_destroy(CommandPackDataHandle self){
    free(self);
    self = NULL;
}
