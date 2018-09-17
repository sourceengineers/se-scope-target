/*!*****************************************************************************
 * @file         trigger.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the Trigger module.
 *               The trigger can have three different configurations:
 *               Continious: The trigger will not trigger.
 *               Normal: The trigger triggers on a given edge and value
 *               OneShot: Same as the normal mode, but stopps the Trigger
 *
 *               To configure the trigger, the specific Trigger has to be passed
 *               to the trigger and the trigger mode has to be choosen. 
 ******************************************************************************/
 
#ifndef TRIGGER_H_
#define TRIGGER_H_

#include <scope/channel.h>

/* Constants to represent the different edges on which the trigger can be 
   configured */
static const int TR_EDGE_POSITIVE = 1;
static const int TR_EDGE_NEGATIVE = -1;

/* Enum to represent different trigger types */
typedef enum TR_MODE_ENUM {TR_NORMAL, TR_CONTINUOUS, TR_ONESHOT} TR_MODE;

/* Struct purely used to configure the channel */
typedef struct {
 float level;
 int edge;
 TR_MODE mode;
 Channel* channel;
} TriggerConfiguration ;


/* Defines class */
/* Define private data */
typedef struct TriggerPrivateStruct TriggerPrivate;
/* Define public data */
typedef struct TriggerStruct Trigger;
typedef bool (*TriggerStrategy)(Trigger* self);

/* Public data struct */
struct TriggerStruct
{
 TriggerPrivate* _private;
 
 TriggerStrategy trigger;
 bool (*configure)(Trigger* self, TriggerConfiguration conf);
};

/******************************************************************************
Public functions 
******************************************************************************/
/* Constructor: Creates a new instanze of the Trigger */
Trigger* Trigger_create();

/* Deconstructor: Deletes the instanze of the Trigger */
void Trigger_destroy(Trigger* self);


static bool Trigger_configure(Trigger* self, TriggerConfiguration conf);

/******************************************************************************
Private functions 
******************************************************************************/

/* Checks if the given configuration data is valid */
static bool Trigger_configSanityCheck(TriggerConfiguration conf);

/* Loads a strategy from the given mode */
static TriggerStrategy Trigger_getTriggerStrategy(Trigger* self, TR_MODE mode);

/* Safes the configuration into the trigger */
static void Trigger_writeConfig(Trigger* self, TriggerConfiguration conf);

/* Checks if the data given is inside the trigger boundaries */
static bool Trigger_checkCurrentData(Trigger* self, float* triggerData);

/* Strategy for the continuous trigger */
static bool Trigger_continuous(Trigger* self);

/* Strategy for the normal trigger */
static bool Trigger_normal(Trigger* self);

/* Strategy for the oneShot trigger */
static bool Trigger_oneShot(Trigger* self);


#endif
