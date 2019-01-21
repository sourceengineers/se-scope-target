/*!*****************************************************************************
 * @file         CommandTrigger.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the cf_trigger command
 *                
 *               Configurates the Trigger. The used structure is defined and 
 *               explained in the trigger.h file.
 ******************************************************************************/

#ifndef COMMANDTRIGGER_H_
#define COMMANDTRIGGER_H_

#include <Scope/Core/Trigger.h>
#include <Scope/Parser/Command/ICommand.h>
#include <Scope/GeneralPurpose/DataTypes.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandTriggerPrivateData* CommandTriggerHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the command */
CommandTriggerHandle CommandTrigger_create(TriggerHandle trigger);

/* Deconstructor: Deletes the instance of the command */
void CommandTrigger_destroy(CommandTriggerHandle self);

/* Sets the Attributes of the command */
void CommandTrigger_setAttributes(CommandTriggerHandle self, TriggerConfiguration conf);

/* Returns the command interface */
ICommandHandle CommandTrigger_getICommand(CommandTriggerHandle self);

#endif
