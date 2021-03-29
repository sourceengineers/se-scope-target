/*!*****************************************************************************
 * @file         CommandPollParser.h
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
 * @brief        Implementation of the configurator for the ev_clear.
 *
 *
 ******************************************************************************/

#ifndef COMMANDPOLLPARSER_H_
#define COMMANDPOLLPARSER_H_

#include "Scope/Control/Commands/Command/CommandPoll.h"
#include "Scope/Control/ICommand.h"
#include "Scope/Core/IScope.h"

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandPollParserPrivateData* CommandPollParserHandle;

/******************************************************************************
 Public functions
******************************************************************************/
/**
 * Constructor
 * @param scope
 * @return
 */
CommandPollParserHandle CommandPollParser_create(IScopeHandle scope, IObserverHandle observer);

/**
 * Returns the command
 * @param self
 * @return
 */
ICommandHandle CommandPollParser_getCommand(CommandPollParserHandle self);

/**
 * Deconstructor
 * @param self
 */
void CommandPollParser_destroy(CommandPollParserHandle self);

#endif
