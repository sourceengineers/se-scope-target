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
 * @brief        Implementation of the cf_trigger command
 *                
 *               Configurates the Trigger. The used structure is defined and 
 *               explained in the trigger.h file.
 ******************************************************************************/

#ifndef COMMANDTRIGGER_H_
#define COMMANDTRIGGER_H_

#include <se-lib-c/util/observer/IObserver.h>
#include "Scope/GeneralPurpose/DataTypes.h"

#include "Scope/Control/ICommand.h"
#include "Scope/Core/IScope.h"
#include "Scope/Core/ScopeTypes.h"

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandTriggerPrivateData* CommandTriggerHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/**
 * Constructor
 * @param scope
 * @return
 */
CommandTriggerHandle CommandTrigger_create(IScopeHandle scope, IObserverHandle observer);

/**
 * Set the attributes which the command will use to configure the scope
 * @param self
 * @param conf
 */
void CommandTrigger_setAttributes(CommandTriggerHandle self, TriggerConfiguration conf);

/**
 * Returns the ICommand interface
 * @param self
 * @return
 */
ICommandHandle CommandTrigger_getICommand(CommandTriggerHandle self);

/**
 * Deconstructor
 * @param self
 */
void CommandTrigger_destroy(CommandTriggerHandle self);

#endif
