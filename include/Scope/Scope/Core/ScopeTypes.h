/*!*****************************************************************************
 * @file         ScopeTypes.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Defines Types used by the Scope and the Parser.
 *
 ******************************************************************************/

#ifndef SCOPETYPES_H
#define SCOPETYPES_H
#include <stdint.h>

/* Trigger definitions */
/* Constants to represent the different edges on which the trigger can be
   configured */
static const int TRIGGER_EDGE_POSITIVE = 1;
static const int TRIGGER_EDGE_NEGATIVE = -1;

/* Enum to represent different trigger types */
typedef enum {TRIGGER_NORMAL, TRIGGER_CONTINUOUS, TRIGGER_ONESHOT} TRIGGER_MODE;

/* Struct purely used to configure the trigger */
typedef struct {
 float level;
 int edge;
 TRIGGER_MODE mode;
 uint32_t channelId;
} TriggerConfiguration;

/* Scope definitions */
typedef enum {TIMESTAMP_AUTOMATIC, TIMESTAMP_MANUAL} TIMESTAMPING_MODE;

/* Channel definitions */
/* Channels States */
typedef enum {CHANNEL_NONE, CHANNEL_INIT, CHANNEL_STOPPED, CHANNEL_RUNNING} CHANNEL_STATES;

#endif
