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

#include <Scope/GeneralPurpose/DataTypes.h>

/* Constants to represent the different edges on which the trigger can be
   configured */
static const int TRIGGER_EDGE_POSITIVE = 1; // Trigger edge == Rising
static const int TRIGGER_EDGE_NEGATIVE = -1; // Trigger edge == Falling

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
    int edge;       // Edge which the trigger detects. This will be either TRIGGER_EDGE_POSITIVE or
                    // TRIGGER_EDGE_NEGATIVE
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
typedef enum{
    PACK_ANNOUNCE, // Announce data will be packed
    PACK_DATA, // Channel data and trigger will be packed
    PACK_DETECT // Detect package that lets hosts discover the device
} PACK_TYPES;

#endif
