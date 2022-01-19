/*!*****************************************************************************
 * @file         CommandParserDispatcher.h
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
 * @brief        Factory to generate the different command interfaces
 * 
 *               Since the usage of malloc after the initial construction is 
 *               not permitted, all objects returned by the commandParserDispatcher are pre-
 *               generated during the construction process.
 *
 ******************************************************************************/

#ifndef COMMANDPARSERDISPATCHER_H_
#define COMMANDPARSERDISPATCHER_H_

#include "Scope/Control/Commands/CommandParser/CommandAddrParser.h"
#include "Scope/Control/Commands/CommandParser/CommandAnnounceParser.h"
#include "Scope/Control/Commands/CommandParser/CommandClearParser.h"
#include "Scope/Control/Commands/CommandParser/CommandPollParser.h"
#include "Scope/Control/Commands/CommandParser/CommandRunningParser.h"
#include "Scope/Control/Commands/CommandParser/CommandTIncParser.h"
#include "Scope/Control/Commands/CommandParser/CommandTransParser.h"
#include "Scope/Control/Commands/CommandParser/CommandTriggerParser.h"
#include "Scope/Control/ICommand.h"
#include "Scope/Control/IUnpacker.h"
#include "Scope/Core/IScope.h"
#include <se-lib-c/util/observer/IObserver.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandParserDispatcherPrivateData* CommandParserDispatcherHandle;

/******************************************************************************
 Public functions 
******************************************************************************/

/**
 * Constructor
 * @param scope
 * @param packObserver
 * @param unpacker
 * @return
 */
CommandParserDispatcherHandle
CommandParserDispatcher_create(IScopeHandle scope, IObserverHandle packObserver, IUnpackerHandle unpacker, size_t amountOfChannels);

/**
 * Returns the command with name matching command.
 * @param self
 * @param command
 * @return Returns NULL if command matches no name of a command
 */
ICommandHandle CommandParserDispatcher_run(CommandParserDispatcherHandle self, MessageType type);

/**
 * Deconstructor
 * @param self
 */
void CommandParserDispatcher_destroy(CommandParserDispatcherHandle self);

#endif
