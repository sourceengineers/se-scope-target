/*!*****************************************************************************
 * @file         ScopeTypes.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
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
