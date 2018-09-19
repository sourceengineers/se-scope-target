/*!*****************************************************************************
 * @file         Trigger.h
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
 *
 *               TODO - OneShot tigger has to be implemented
 *
 ******************************************************************************/
 
#ifndef TRIGGER_H_
#define TRIGGER_H_

#include <Scope/Channel.h>

/* Constants to represent the different edges on which the trigger can be 
   configured */
static const int TRIGGER_EDGE_POSITIVE = 1;
static const int TRIGGER_EDGE_NEGATIVE = -1;

/* Enum to represent different trigger types */
typedef enum {TRIGGER_NORMAL, TRIGGER_CONTINUOUS, TRIGGER_ONESHOT} TRIGGER_MODE;

/* Struct purely used to configure the channel */
typedef struct {
 float level;
 int edge;
 TRIGGER_MODE mode;
 IFloatStream stream;
} TriggerConfiguration ;


/* Defines class */
typedef struct __TriggerPrivateData* TriggerHandle;

/* Typedef for a function pointer, to easier handle the strategies */
typedef bool (*TriggerStrategy)(TriggerHandle self, const int index);
/* Function pointer to the currently implemented strategy */
TriggerStrategy Trigger_run;
/******************************************************************************
Public functions 
******************************************************************************/
/* Constructor: Creates a new instanze of the Trigger */
TriggerHandle Trigger_create();

/* Deconstructor: Deletes the instanze of the Trigger */
void Trigger_destroy(TriggerHandle self);

/* Configures the trigger */
bool Trigger_configure(TriggerHandle self, TriggerConfiguration conf);

/* Returns the triggered index */
int Trigger_getTriggerIndex(TriggerHandle self);
#endif
