/*!*****************************************************************************
 * @file         CommandTInc.h
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
 * @brief        Implementation of the cf_t_inc command
 *                
 *               Sets the time increment of the scope poll events. 
 *               The function will be provided by the IScope interface, to 
 *               prevent code dublication.
 *                
 ******************************************************************************/

#ifndef COMMANDTINC_H_
#define COMMANDTINC_H_

#include "Scope/GeneralPurpose/DataTypes.h"

#include "Scope/Control/ICommand.h"
#include "Scope/Core/IScope.h"

#include <stdint.h>
#include <se-lib-c/util/observer/IObserver.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandTIncPrivateData* CommandTIncHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/**
 * Constructor
 * @param scope
 * @return
 */
CommandTIncHandle CommandTInc_create(IScopeHandle scope, IObserverHandle observer);

/**
 * Set the attributes which the command will use to configure the scope
 * @param self
 * @param timeIncrement
 */
void CommandTInc_setAttributes(CommandTIncHandle self, uint32_t timeIncrement);

/**
 * Returns the ICommand interface
 * @param self
 * @return
 */
ICommandHandle CommandTInc_getICommand(CommandTIncHandle self);

/**
 * Deconstructor
 * @param self
 */
void CommandTInc_destroy(CommandTIncHandle self);

#endif
