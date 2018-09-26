/*!*****************************************************************************
 * @file         CommandTrigger.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the cf_trigger command
 *                
 *               
 ******************************************************************************/
 
#ifndef COMMANDTRIGGER_H_
#define COMMANDTRIGGER_H_

#include <Scope/Trigger.h>
#include <Command/ICommand.h>

/* Defines class */
typedef struct __CommandTriggerPrivateData* CommandTriggerHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instanze of the channel */
CommandTriggerHandle CommandTrigger_create(TriggerHandle trigger);

/* Deconstructor: Deletes the instanze of the channel */
void CommandTrigger_destroy(CommandTriggerHandle self);

/* Returns the command interface */
ICommandHandle CommandTrigger_getICommand(CommandTriggerHandle self);

/* Returns the name of the command */
const char* CommandTrigger_getName(CommandTriggerHandle self);


#endif
