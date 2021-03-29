/*!*****************************************************************************
 * @file         CommandPackAnnounceParser.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @license      This file is part of SE-Scope.
 *
 *               SE-Scope is free software; you can redistribute it and/or
 *               modify it under the terms of the GNU General Public License as
 *               published by the Free Software Foundation.
 *
 *               SE-Scope is distributed in the hope that it will be useful,
 *               but WITHOUT ANY WARRANTY; without even the implied warranty of
 *               MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *               GNU General Public License for more details.
 *
 *               You should have received a copy of the GNU General Public License
 *               along with SE-Scope.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the configurator for the ev_pack.
 *
 *              
 ******************************************************************************/

#ifndef COMMANDPACKANNOUNCEPARSER_H_
#define COMMANDPACKANNOUNCEPARSER_H_

#include <se-lib-c/util/observer/IObserver.h>
#include "Scope/Control/AnnounceStorage.h"

#include "Scope/Control/PackCommands/Command/CommandPackAnnounce.h"
#include "Scope/Control/ICommand.h"
#include "Scope/Control/IPacker.h"
#include "Scope/Core/IScope.h"

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandPackAnnounceParserPrivateData* CommandPackAnnounceParserHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/**
 * Constructor
 * @param announceStorage
 * @param packer
 * @return
 */
CommandPackAnnounceParserHandle CommandPackAnnounceParser_create(AnnounceStorageHandle announceStorage, IPackerHandle packer);

/**
 * Returns the command
 * @param self
 * @return
 */
ICommandHandle CommandPackAnnounceParser_getCommand(CommandPackAnnounceParserHandle self);

/**
 * Deconstructor
 * @param self
 */
void CommandPackAnnounceParser_destroy(CommandPackAnnounceParserHandle self);

#endif
