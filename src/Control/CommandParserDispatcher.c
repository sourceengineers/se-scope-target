/*!****************************************************************************************************************************************
 * @file         CommandParserDispatcher.c
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
 * @authors      Samuel Schuepbach <samuel.schuepbach@sourceengineers.com>
 *
 *****************************************************************************************************************************************/

#include "Scope/Control/CommandParserDispatcher.h"
#include "Scope/Control/Commands/CommandParser/CommandAddrParser.h"
#include "Scope/Control/Commands/CommandParser/CommandAnnounceParser.h"
#include "Scope/Control/Commands/CommandParser/CommandClearParser.h"
#include "Scope/Control/Commands/CommandParser/CommandPollParser.h"
#include "Scope/Control/Commands/CommandParser/CommandRunningParser.h"
#include "Scope/Control/Commands/CommandParser/CommandTIncParser.h"
#include "Scope/Control/Commands/CommandParser/CommandTransParser.h"
#include "Scope/Control/Commands/CommandParser/CommandTriggerParser.h"
#include "Scope/Control/Commands/CommandParser/CommandDetectParser.h"
#include "Scope/Control/ParserDefinitions.h"
#include "Scope/Core/ScopeTypes.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandParserDispatcherPrivateData {
    CommandAddrParserHandle commandAddrParser;
    CommandAnnounceParserHandle commandAnnounceParser;
    CommandClearParserHandle commandClearParser;
    CommandDetectParserHandle commandDetectParser;
    CommandPollParserHandle commandPollParser;
    CommandRunningParserHandle commandRunningParser;
    CommandTIncParserHandle commandTIncParser;
    CommandTransParserHandle commandTransParser;
    CommandTriggerParserHandle commandTriggerParser;
} CommandParserDispatcherPrivateData;


/******************************************************************************
 Public functions
******************************************************************************/
CommandParserDispatcherHandle
CommandParserDispatcher_create(IScopeHandle scope, IObserverHandle packObserver, IUnpackerHandle unpacker) {

    CommandParserDispatcherHandle self = malloc(sizeof(CommandParserDispatcherPrivateData));
    assert(self);

    /* Initialize needed command parser
    * Not all commands need parser, which is why there are less parser as commands */
    self->commandAddrParser = CommandAddrParser_create(scope, unpacker, packObserver);
    self->commandAnnounceParser = CommandAnnounceParser_create(packObserver);
    self->commandClearParser = CommandClearParser_create(scope, packObserver);
    self->commandDetectParser = CommandDetectParser_create(packObserver);
    self->commandPollParser = CommandPollParser_create(scope, packObserver);
    self->commandRunningParser = CommandRunningParser_create(scope, unpacker, packObserver);
    self->commandTIncParser = CommandTIncParser_create(scope, unpacker, packObserver);
    self->commandTransParser = CommandTransParser_create(scope, packObserver);
    self->commandTriggerParser = CommandTriggerParser_create(scope, unpacker, packObserver);

    return self;
}

ICommandHandle CommandParserDispatcher_run(CommandParserDispatcherHandle self, MessageType type) {

    if (type == CF_RUNNING) {
        return CommandRunningParser_getCommand(self->commandRunningParser);
    } else if (type == EV_POLL) {
        return CommandPollParser_getCommand(self->commandPollParser);
    } else if (type == CF_ADDR) {
        return CommandAddrParser_getCommand(self->commandAddrParser);
    } else if (type == CF_T_INC) {
        return CommandTIncParser_getCommand(self->commandTIncParser);
    } else if (type == EV_TRANS) {
        return CommandTransParser_getCommand(self->commandTransParser);
    } else if (type == CF_TRIGGER) {
        return CommandTriggerParser_getCommand(self->commandTriggerParser);
    } else if (type == EV_ANNOUNCE) {
        return CommandAnnounceParser_getCommand(self->commandAnnounceParser);
    } else if (type == EV_CLEAR) {
        return CommandClearParser_getCommand(self->commandClearParser);
    } else if (type == EV_DETECT) {
        return CommandDetectParser_getCommand(self->commandDetectParser);
    }

    return NULL;
}

void CommandParserDispatcher_destroy(CommandParserDispatcherHandle self) {

    CommandRunningParser_destroy(self->commandRunningParser);
    CommandAddrParser_destroy(self->commandAddrParser);
    CommandTIncParser_destroy(self->commandTIncParser);
    CommandTriggerParser_destroy(self->commandTriggerParser);
    CommandPollParser_destroy(self->commandPollParser);
    CommandClearParser_destroy(self->commandClearParser);
    CommandAnnounceParser_destroy(self->commandAnnounceParser);
    CommandTransParser_destroy(self->commandTransParser);
    CommandDetectParser_destroy(self->commandDetectParser);
    free(self);
    self = NULL;
}
