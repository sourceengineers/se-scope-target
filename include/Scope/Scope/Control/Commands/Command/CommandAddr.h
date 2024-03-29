/*!*****************************************************************************
 * @file         CommandAddr.h
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
 * @brief        Implementation of the cf_addr command
 *                
 *               To configure the command, the CommandAddrConf Object has to be 
 *               passed to the setCommandAttribute function.
 *                
 *               The field numberOfChangedChannels is indicating how many
 *               many channels will be changed. The field changedChannels 
 *               contains the channels which will be changed. The field 
 *               newAddresses contains the new addresses and types the machting 
 *               types of addresses to which will be fed into the channels.
 *
 *               All of these fields cannot be longer than 
 *               numberOfChangedChannels or it will be ingored.
 *              
 ******************************************************************************/

#ifndef COMMANDADDR_H_
#define COMMANDADDR_H_

#include <stdint.h>

#include <se-lib-c/util/observer/IObserver.h>

#include "Scope/GeneralPurpose/DataTypes.h"
#include "Scope/Control/ICommand.h"
#include "Scope/Core/IScope.h"


/* Definition of the configuration struct */
/**
 * Struct used to communicate with its parser. Contains definitions of how and which addresses have to be
 * configured
 */
typedef struct{
    void** newAddresses;          // Array to keep track on witch channels have changed
    uint32_t* changedChannels;         // Id's of the channels stored in "newAddresses"
    uint32_t numberOfChangedChannels;
    DATA_TYPES* types;            // Types of the channels
} CommandAddrConf;

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandAddrPrivateData* CommandAddrHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/**
 * Constructor
 * @param scope Dependency to the scope interface
 * @return
 */
CommandAddrHandle CommandAddr_create(IScopeHandle scope, IObserverHandle observer);

/**
 * Set the attributes which the command will use to configure the scope
 * @param self
 * @param conf
 */
void CommandAddr_setAttributes(CommandAddrHandle self, CommandAddrConf conf);

/**
 * Returns the ICommand interface
 * @param self
 * @return
 */
ICommandHandle CommandAddr_getICommand(CommandAddrHandle self);

/**
 * Deconstructor
 * @param self
 */
void CommandAddr_destroy(CommandAddrHandle self);

#endif
