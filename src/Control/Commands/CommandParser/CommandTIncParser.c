/*!****************************************************************************************************************************************
 * @file         CommandTIncParser.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include "Scope/Control/Commands/CommandParser/CommandTIncParser.h"
#include "Scope/Control/Commands/Command/CommandTInc.h"
#include "Scope/Control/IUnpacker.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Name of the command */
static char* commandName = "cf_t_inc";

/* Class data */
typedef struct __CommandTIncParserPrivateData{
    CommandTIncHandle command;

    IUnpackerHandle unpacker;

} CommandTIncParserPrivateData;

/******************************************************************************
 Public functions
******************************************************************************/
CommandTIncParserHandle CommandTIncParser_create(IScopeHandle scope, IUnpackerHandle unpacker,
        IObserverHandle observer){
    CommandTIncParserHandle self = malloc(sizeof(CommandTIncParserPrivateData));
    assert(self);

    self->command = CommandTInc_create(scope, observer);
    self->unpacker = unpacker;
    return self;
}

ICommandHandle CommandTIncParser_getCommand(CommandTIncParserHandle self){

    if(self->unpacker == NULL){
        return NULL;
    }

    const uint32_t timeIncrement = self->unpacker->cfTInc_getInc(self->unpacker);
    CommandTInc_setAttributes(self->command, timeIncrement);
    return CommandTInc_getICommand(self->command);
}

void CommandTIncParser_destroy(CommandTIncParserHandle self){
    CommandTInc_destroy(self->command);

    free(self);
    self = NULL;
}
