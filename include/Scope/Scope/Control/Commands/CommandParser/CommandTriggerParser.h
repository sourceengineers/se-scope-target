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

#include "Scope/Control/Commands/Command/CommandTrigger.h"
#include "Scope/Control/IUnpacker.h"
#include "Scope/Control/ICommand.h"
#include "Scope/Core/IScope.h"

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandTriggerParserPrivateData* CommandTriggerParserHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/**
 * Constructor
 * @param scope
 * @param unpacker
 * @return
 */
CommandTriggerParserHandle CommandTriggerParser_create(IScopeHandle scope, IUnpackerHandle unpacker);

/**
 * Returns the command
 * @param self
 * @return
 */
ICommandHandle CommandTriggerParser_getCommand(CommandTriggerParserHandle self);

/**
 * Returns the name of the command
 * @return
 */
char* CommandTriggerParser_getName(void);

/**
 * Deconstructor
 * @param self
 */
void CommandTriggerParser_destroy(CommandTriggerParserHandle self);

#endif
