/*!*****************************************************************************
 * @file         CommandRunning.h
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
