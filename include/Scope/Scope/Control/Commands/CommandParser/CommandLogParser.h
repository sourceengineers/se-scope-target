/*!*****************************************************************************
 * @file         CommandLogParser.h
 *
 * @copyright    Copyright (c) 2020 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Anselm Fuhrer - anselm.fuhrer@sourceengineers.com
 *
 * @brief        TODO
 *
 *
 ******************************************************************************/

#ifndef COMMANDLOGPARSER_H_
#define COMMANDLOGPARSER_H_

#include "Scope/Control/Commands/Command/CommandLog.h"
#include "Scope/Control/ICommand.h"
#include "Scope/Core/IScope.h"
#include <se-lib-c/util/observer/IObserver.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandLogParserPrivateData* CommandLogParserHandle;

/******************************************************************************
 Public functions
******************************************************************************/
/**
 * Constructor
 * @param packObserver
 * @return
 */
CommandLogParserHandle CommandLogParser_create(IObserverHandle packObserver);

/**
 * Returns the command
 * @param self
 * @return
 */
ICommandHandle CommandLogParser_getCommand(CommandLogParserHandle self);

/**
 * Deconstructor
 * @param self
 */
 void CommandLogParser_destroy(CommandLogParserHandle self);

#endif
