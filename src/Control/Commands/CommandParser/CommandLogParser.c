/*!****************************************************************************************************************************************
 * @file         CommandLogParser.c
 *
 * @copyright    Copyright (c) 2020 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Anselm Fuhrer anselm.fuhrer@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include "Scope/GeneralPurpose/DataTypes.h"

#include "Scope/Control/Commands/CommandParser/CommandLogParser.h"
#include "Scope/Control/IUnpacker.h"
#include "Scope/Control/ParserDefinitions.h"
#include <stdlib.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandLogParserPrivateData
{
    CommandLogHandle command;

    IUnpackerHandle unpacker;

} CommandLogParserPrivateData;

/******************************************************************************
 Private functions
******************************************************************************/

/******************************************************************************
 Public functions
******************************************************************************/
CommandLogParserHandle
CommandLogParser_create(IObserverHandle observer)
{
    //TODO implement for LogParser
	CommandLogParserHandle self = malloc(sizeof(CommandLogParserPrivateData));

    assert(self);
    self->command = CommandLog_create(observer);
    return self;
}

//TODO implement
ICommandHandle CommandLogParser_getCommand(CommandLogParserHandle self)
{

    if(self->unpacker == NULL){
        return NULL;
    }
    return NULL;	//TODO
    //return CommandLog_getICommand(self->command);

}

void CommandLogParser_destroy(CommandLogParserHandle self)
{
	CommandLog_destroy(self->command);
    free(self);
    self = NULL;
}
