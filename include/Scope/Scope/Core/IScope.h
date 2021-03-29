/*!*****************************************************************************
 * @file         IScope.h
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
 * @brief        Interface for functions which will be used to communicate with the control layer.
 *
 ******************************************************************************/

#ifndef ISCOPE_H_
#define ISCOPE_H_

#include <se-lib-c/stream/IIntStream.h>
#include <se-lib-c/container/FloatRingBuffer.h>

#include "Scope/Core/ScopeTypes.h"
#include "Scope/GeneralPurpose/DataTypes.h"

/******************************************************************************
 Define interface handle data
******************************************************************************/
typedef struct IScopeStruct* IScopeHandle;

/******************************************************************************
 Define interface
******************************************************************************/
typedef struct IScopeStruct{
    SeScopeGenericReference handle;

    /**
     * Tells the scope to poll data in the active channels
     * Corresponding command: ev_poll
     * @param scope
     */
    void (* poll)(IScopeHandle scope);

    /**
     * Sets the time increment between poll events.
     * Corresponding command: cf_t_inc
     * @param scope
     * @param timeIncrement
     */
    void (* setTimeIncrement)(IScopeHandle scope, uint32_t timeIncrement);

    /**
     * Returns the time increment which is set between poll events
     * @param scope
     * @return
     */
    uint32_t (* getTimeIncrement)(IScopeHandle scope);

    /**
     * Returns a reference to the timestamp stream
     * @param scope
     * @return
     */
    IIntStreamHandle (* getTimestamp)(IScopeHandle scope);

    /**
     * Clears the timestamp stream as well as all channels
     * Corresponding command: ev_clear
     * @param scope
     */
    void (* clear)(IScopeHandle scope);

    /**
     * Configures the trigger. Used for cf_trigger
     * @param scope
     * @param conf
     */
    void (* configureTrigger)(IScopeHandle scope, TriggerConfiguration conf);

    /**
     * Configurates the channels of a specific address.
     * Corresponding command: cf_addr
     * @param scope
     * @param address Address which the channel will poll from
     * @param idOfChangedChannel If idOfChangedChannel > max amount of channels, the command gets rejected.
     * @param typeOfAddress Type of the address
     */
    void (* configureChannelAddress)(IScopeHandle scope, void* address,
                                     uint32_t idOfChangedChannel, DATA_TYPES typeOfAddress);

    /**
     * Commands the scope to transmit its data
     * Corresponding command: ev_trans
     * @param scope
     */
    void (* transmit)(IScopeHandle scope);

    /**
     * Starts the channel with its id matching idOfChangedChannel
     * Corresponding command: cf_running (true)
     * @param scope
     * @param idOfChangedChannel If idOfChangedChannel > max amount of channels, the command gets rejected.
     */
    void (* setChannelRunning)(IScopeHandle scope, uint32_t idOfChangedChannel);

    /**
     * Stopps the channel with its id matching idOfChangedChannel
     * Corresponding command: cf_running (false)
     * @param scope
     * @param idOfChangedChannel If idOfChangedChannel > max amount of channels, the command gets rejected.
     */
    void (* setChannelStopped)(IScopeHandle scope, uint32_t idOfChangedChannel);

    /**
     * Returns the max amount of channels configured
     * @param scope
     * @return
     */
    size_t (* getAmountOfChannels)(IScopeHandle scope);

    /**
     * Returns the data in the trigger. Such as triggered or not, timestamp when triggered and activated channel
     * @param scope
     * @return
     */
    TriggeredValues (* getTriggerData)(IScopeHandle scope);

    /**
     * Checks if a specific channel is ready to be packed
     * @param scope
     * @param channelId  If channelId > max amount of channels, the command gets rejected.
     * @return
     */
    bool (* channelHasToBePacked)(IScopeHandle scope, uint32_t channelId);

    /**
     * Returns a reference to a FloatRingBuffer of the channel matching channelId
     * @param scope
     * @param channelId If channelId > max amount of channels, the command gets rejected.
     * @return
     */
    FloatRingBufferHandle (* getChannelBuffer)(IScopeHandle scope, uint32_t channelId);

    /**
     * Returns if the scope is currently running or not
     * @param scope
     * @return
     */
    bool (*isRunning)(IScopeHandle scope);

    /**
     * Returns if the scope is ready to run or not
     * @param scope
     * @return
     */
    bool (*isReadyToRun)(IScopeHandle scope);

} IScope;

#endif
