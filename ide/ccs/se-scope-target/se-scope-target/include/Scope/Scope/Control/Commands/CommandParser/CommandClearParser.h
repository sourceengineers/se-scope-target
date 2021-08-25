/*!*****************************************************************************
 * @file         CommandClearParser.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the configurator for the ev_clear.
 *
 *              
 ******************************************************************************/

#ifndef COMMANDCLEARPARSER_H_
#define COMMANDCLEARPARSER_H_

#include <se-lib-c/util/observer/IObserver.h>
#include "Scope/Control/Commands/Command/CommandClear.h"
#include "Scope/Control/ICommand.h"
#include "Scope/Core/IScope.h"

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandClearParserPrivateData* CommandClearParserHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/**
 * Constructor
 * @param scope
 * @return
 */
CommandClearParserHandle CommandClearParser_create(IScopeHandle scope, IObserverHandle packObserver);

/**
 * Returns the command
 * @param self
 * @return
 */
ICommandHandle CommandClearParser_getCommand(CommandClearParserHandle self);

/**
 * Deconstructor
 * @param self
 */
void CommandClearParser_destroy(CommandClearParserHandle self);

#endif
