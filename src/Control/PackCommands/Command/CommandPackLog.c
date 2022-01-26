/*!****************************************************************************************************************************************
 * @file         CommandPackLog.c
 *
 * @copyright    Copyright (c) 2021 by Source Engineers GmbH. All Rights Reserved.
 *
 * @license {    This file is part of se-scope-target.
 *
 *               se-scope-target is free software; you can redistribute it and/or
 *               modify it under the terms of the GPLv3 General Public License Version 3
 *               as published by the Free Software Foundation.
 *
 *               se-scope-target is distributed in the hope that it will be useful,
 *               but WITHOUT ANY WARRANTY; without even the implied warranty of
 *               MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *               GNU General Public License for more details.
 *
 *               You should have received a copy of the GPLv3 General Public License Version 3
 *               along with se-scope-target.  If not, see <http://www.gnu.org/licenses/>.
 *
 *               In closed source or commercial projects, GPLv3 General Public License Version 3
 *               is not valid. In this case the commercial license received with the purchase
 *               is applied (See SeScopeLicense.pdf).
 *               Please contact us at scope@sourceengineers.com for a commercial license.
 * }
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

#define MAX_MESSAGE_LENGTH 50

static void run(ICommandHandle command){
    CommandPackLogHandle self = (CommandPackLogHandle) command->handle;
    ScLogDataDef logStreamData;

    char msg[MAX_MESSAGE_LENGTH];
    // how many bytes are in the buffer
    size_t length = self->logStream->length(self->logStream);
    if (length > MAX_MESSAGE_LENGTH - 2){
        length = MAX_MESSAGE_LENGTH - 2;
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
