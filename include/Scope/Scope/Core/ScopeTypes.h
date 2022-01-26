/*!*****************************************************************************
 * @file         ScopeTypes.h
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
 * @brief        Defines Types used by the Scope.
 *
 ******************************************************************************/

#ifndef SCOPETYPES_H
#define SCOPETYPES_H

#include "Scope/GeneralPurpose/DataTypes.h"

/**
 * Enum to represent different trigger types
 * */
typedef enum{
    TRIGGER_NORMAL, // Trigger detects a specified edge and level.
    TRIGGER_CONTINUOUS, // Trigger polls constantly and transmits the data as soon as the channel is full
    TRIGGER_ONESHOT // Same as TRIGGER_NORMAL, but shuts down after a trigger event.
} TRIGGER_MODE;

/**
 * Struct purely used to configure the trigger
 */
typedef struct{
    float level;    // Level on which the trigger detects for
    bool edge;       // Edge which the trigger detects. False -> Negative, True -> Positive
    TRIGGER_MODE mode; // Mode which the trigger runs in
    uint32_t channelId; // Channel which the trigger should monitor
} TriggerConfiguration;

/**
 * Struct used to transmit the data in the trigger
 */
typedef struct{
    bool isTriggered; // True if the trigger is triggered
    uint32_t channelId; // Channel id which the trigger currently is monitoring
    uint32_t triggerTimestamp; // Timestamp at which the trigger detected matching trigger criteria
    TRIGGER_MODE mode;
} TriggeredValues;

/**
 * Channel states
 */
typedef enum{
    CHANNEL_NONE, // undefined state
    CHANNEL_INIT, // Channel is waiting to be configured
    CHANNEL_STOPPED, // Channel is stopped and won't poll data
    CHANNEL_RUNNING // Channel is running and will poll data
} CHANNEL_STATES;

/**
 * Enum to determine the source of the observer.
 */
typedef enum __MessageType {

    SE_NONE = 0,
    SE_NAK = 1,
    SE_ACK = 2,

    SC_DATA = 50,
    SC_ANNOUNCE = 51,
    SC_DETECT = 52,
    SC_STREAM = 53,
    SC_LOG = 54,

    EV_ANNOUNCE = 100,
    EV_CLEAR = 101,
    EV_POLL = 102,
    EV_TRANS = 103,
    EV_DETECT = 104,
    CF_RUNNING = 105,
    CF_TRIGGER = 106,
    CF_T_INC = 107,
    CF_ADDR = 108,

} MessageType;

#define ENUM_END_CONTROL_FLOW SE_ACK
#define ENUM_START_CLIENT_TO_HOST SC_DATA
#define ENUM_END_CLIENT_TO_HOST SC_LOG
#define NUM_CLIENT_TO_HOST_MESSAGES (size_t)(ENUM_END_CLIENT_TO_HOST - ENUM_START_CLIENT_TO_HOST + 1 + ENUM_END_CONTROL_FLOW)

#define ENUM_START_HOST_TO_CLIENT EV_ANNOUNCE


#endif
