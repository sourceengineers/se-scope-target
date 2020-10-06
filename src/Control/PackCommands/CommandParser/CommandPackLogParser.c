/*!****************************************************************************************************************************************
 * @file         CommandPackLogParser.c
 *
 * @copyright    Copyright (c) 2020 by Sourceengineers. All Rights Reserved.
 *
 * @authors     Anselm Fuhrer anselm.fuhrer@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include "Scope/Control/PackCommands/CommandParser/CommandPackLogParser.h"
#include "Scope/Control/PackCommands/Command/CommandPackLog.h"

#include <stdlib.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandPackLogParserPrivateData{
	CommandPackLogHandle command;
} CommandPackLogParserPrivateData;

/******************************************************************************
 Public functions
******************************************************************************/
CommandPackLogParserHandle CommandPackLogParser_create(ScLogDataDef log, IPackerHandle packer){
    CommandPackLogParserHandle self = malloc(sizeof(CommandPackLogParserPrivateData));
    assert(self);
    self->command = CommandPackLog_create(log, packer);
    return self;
}

ICommandHandle CommandPackLogParser_getCommand(CommandPackLogParserHandle self){
    return CommandPackLog_getICommand(self->command);
}

void CommandPackLogParser_destroy(CommandPackLogParserHandle self){
    CommandPackLog_destroy(self->command);
    free(self);
    self = NULL;
}
