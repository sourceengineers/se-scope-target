/*!*****************************************************************************
 * @file         CommandAnnounce.h
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
 * @brief        Implementation of the ev_announce command
 *                
 *               Starts a sending sending the pending addresses
 * 
 ******************************************************************************/

#ifndef COMMANDANNOUNCE_H_
#define COMMANDANNOUNCE_H_

#include <se-lib-c/util/observer/IObserver.h>

#include "Scope/Control/ICommand.h"
#include "Scope/Core/IScope.h"


/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandAnnouncePrivateData* CommandAnnounceHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/**
 * Constructor
 * @param packObserver Dependency to the pack observer which will be updated during the execution of the command
 * @return
 */
CommandAnnounceHandle CommandAnnounce_create(IObserverHandle packObserver);

/**
 * Returns the ICommand interface
 * @param self
 * @return
 */
ICommandHandle CommandAnnounce_getICommand(CommandAnnounceHandle self);

/**
 * Deconstructor
 * @param self
 */
void CommandAnnounce_destroy(CommandAnnounceHandle self);
#endif
