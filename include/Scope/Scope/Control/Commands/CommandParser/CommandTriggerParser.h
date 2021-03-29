/*!*****************************************************************************
 * @file         CommandTrigger.h
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
 * @brief        Implementation of the configurator for the cf_Trigger.
 *
 *              
 ******************************************************************************/

#ifndef COMMANDTRIGGERPARSER_H_
#define COMMANDTRIGGERPARSER_H_

#include "Scope/Control/Commands/Command/CommandTrigger.h"
#include "Scope/Control/IUnpacker.h"
#include "Scope/Control/ICommand.h"
#include "Scope/Core/IScope.h"

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandTriggerParserPrivateData* CommandTriggerParserHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/**
 * Constructor
 * @param scope
 * @param unpacker
 * @return
 */
CommandTriggerParserHandle CommandTriggerParser_create(IScopeHandle scope, IUnpackerHandle unpacker,
        IObserverHandle observer);

/**
 * Returns the command
 * @param self
 * @return
 */
ICommandHandle CommandTriggerParser_getCommand(CommandTriggerParserHandle self);

/**
 * Deconstructor
 * @param self
 */
void CommandTriggerParser_destroy(CommandTriggerParserHandle self);

#endif
