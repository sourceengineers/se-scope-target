/*!*****************************************************************************
 * @file         CommandPackParserDispatcher.h
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

#ifndef COMMANDPACKPARSERDISPATCHER_H_
#define COMMANDPACKPARSERDISPATCHER_H_

#include "Scope/Control/AnnounceStorage.h"

#include "Scope/Control/PackCommands/CommandParser/CommandPackAnnounceParser.h"
#include "Scope/Control/PackCommands/CommandParser/CommandPackDataParser.h"
#include "Scope/Control/ICommand.h"
#include "Scope/Control/IPacker.h"
#include "Scope/Core/IScope.h"

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandPackParserDispatcherPrivateData* CommandPackParserDispatcherHandle;

/******************************************************************************
 Public functions 
******************************************************************************/

/**
 * Constructor
 * @param scope
 * @param announceStorage
 * @param packer
 * @return
 */
CommandPackParserDispatcherHandle
CommandPackParserDispatcher_create(IScopeHandle scope, AnnounceStorageHandle announceStorage, IPackerHandle packer, IByteStreamHandle logStream);

/**
 * Returns the command with name matching command.
 * @param self
 * @param command
 * @return Returns NULL if command matches no name of a command
 */
ICommandHandle CommandPackParserDispatcher_run(CommandPackParserDispatcherHandle self, MessageType type);

/**
 * Deconstructor
 * @param self
 */
void CommandPackParserDispatcher_destroy(CommandPackParserDispatcherHandle self);

#endif
