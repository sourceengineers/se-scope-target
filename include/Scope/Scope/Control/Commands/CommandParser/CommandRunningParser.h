/*!*****************************************************************************
 * @file         CommandRunning.h
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
 *               is not valid. In these cases the purchased commercial license is applied.
 *               Please contact us at scope@sourceengineers.com for a commercial license.
 * }
 *
 * @authors      Samuel Schuepbach <samuel.schuepbach@sourceengineers.com>
 *
 * @brief        Implementation of the configurator for the cf_running.
 *
 *              
 ******************************************************************************/

#ifndef COMMANDRUNNINGPARSER_H_
#define COMMANDRUNNINGPARSER_H_

#include "Scope/Control/Commands/Command/CommandRunning.h"
#include "Scope/Control/IUnpacker.h"
#include "Scope/Control/ICommand.h"
#include "Scope/Core/IScope.h"

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandRunningParserPrivateData* CommandRunningParserHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/**
 * Constructor
 * @param scope
 * @param unpacker
 * @return
 */
CommandRunningParserHandle CommandRunningParser_create(IScopeHandle scope,
                                                       IUnpackerHandle unpacker,
                                                       IObserverHandle observer,
                                                       size_t amountOfChannels);

/**
 * Returns the command
 * @param self
 * @return
 */
ICommandHandle CommandRunningParser_getCommand(CommandRunningParserHandle self);

/**
 * Deconstructor
 * @param self
 */
void CommandRunningParser_destroy(CommandRunningParserHandle self);

#endif
