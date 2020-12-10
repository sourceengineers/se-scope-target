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
#include <string.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandPackLogPrivateData{
    ICommand command;
    IPackerHandle packer;
    IByteStreamHandle logStream;
} CommandPackLogPrivateData;


static void run(ICommandHandle command);


/******************************************************************************
 Private functions
******************************************************************************/

static void run(ICommandHandle command){
	CommandPackLogHandle self = (CommandPackLogHandle) command->handle;
	ScLogDataDef logStreamData;
	size_t lengthOfOneMessage = 50;

	char msg[lengthOfOneMessage];
	// how many bytes are in the buffer
	size_t length = self->logStream->length(self->logStream);
	if (length > lengthOfOneMessage-2){
		length = lengthOfOneMessage-2;
	}
	self->logStream->read(self->logStream, (uint8_t *) &msg, length);
	msg[length] = '\0';
	memcpy((uint8_t *) logStreamData.message, (uint8_t *) msg, (length+1) * sizeof(msg[0]));
	self->packer->addLog(self->packer, logStreamData);
}


/******************************************************************************
 Private functions
******************************************************************************/
CommandPackLogHandle CommandPackLog_create(IByteStreamHandle logStream, IPackerHandle packer){
    CommandPackLogHandle self = malloc(sizeof(CommandPackLogPrivateData));
    assert(self);

    self->logStream = logStream;
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
