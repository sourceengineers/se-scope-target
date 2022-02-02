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
 * @brief        Implementation of the running command.
 *               
 *               As Attribute interface, the CommandRunningConfig struct is used
 *               If the channel state has changed, it has to be flagged in the 
 *               channelHasChanged array. 
 *               If the channels were flagged, their state will be changed when 
 *               the run() function is executed, according to the setRunning 
 *               array.
 *               Only CHANNEL_RUNNING and CHANNEL_STOPPED will affect the
 *               channel state.
 ******************************************************************************/

#ifndef COMMANDRUNNING_H_
#define COMMANDRUNNING_H_

#include "Scope/GeneralPurpose/DataTypes.h"
#include <se-lib-c/util/observer/IObserver.h>

#include "Scope/Control/ICommand.h"
#include "Scope/Core/IScope.h"
#include "Scope/Core/ScopeTypes.h"

#include <stdint.h>
#include <stdbool.h>

/* Struct used to help the command configure the channels */
typedef struct{
    CHANNEL_STATES* newStates;
    uint32_t* changedChannels;
    uint32_t numberOfChangedChannels;
    bool triggerChanged;
    bool triggerRunning;
} CommandRunningConf;

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandRunningPrivateData* CommandRunningHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/**
 * Constructor
 * @param scope
 * @return
 */
CommandRunningHandle CommandRunning_create(IScopeHandle scope, IObserverHandle observer);

/**
 * Set the attributes which the command will use to configure the scope
 * @param self
 * @param conf
 */
void CommandRunning_setAttributes(CommandRunningHandle self, CommandRunningConf conf);

/**
 * Returns the ICommand interface
 * @param self
 * @return
 */
ICommandHandle CommandRunning_getICommand(CommandRunningHandle self);

/**
 * Deconstructor
 * @param self
 */
void CommandRunning_destroy(CommandRunningHandle self);

#endif
