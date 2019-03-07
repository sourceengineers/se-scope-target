/*!*****************************************************************************
 * @file         CommandTrigger.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the configurator for the cf_Trigger.
 *
 *              
 ******************************************************************************/

#ifndef COMMANDTRIGGERPARSER_H_
#define COMMANDTRIGGERPARSER_H_

#include <Scope/Control/Command/CommandTrigger.h>
#include <Scope/Control/IUnpacker.h>
#include <Scope/Control/Command/ICommand.h>
#include <Scope/Core/IScope.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandTriggerParserPrivateData* CommandTriggerParserHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the command parser */
CommandTriggerParserHandle CommandTriggerParser_create(IScopeHandle scope, IUnpackerHandle unpacker);

/* Configurates the gives command */
ICommandHandle CommandTriggerParser_getCommand(CommandTriggerParserHandle self);

/* Returns the name of the command */
char* CommandTriggerParser_getName(void);

/* Deconstructor: Deletes the instance of the command parser */
void CommandTriggerParser_destroy(CommandTriggerParserHandle self);

#endif
