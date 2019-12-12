/*!*****************************************************************************
 * @file         CommandRunning.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the configurator for the cf_running.
 *
 *              
 ******************************************************************************/

#ifndef COMMANDRUNNINGPARSER_H_
#define COMMANDRUNNINGPARSER_H_

#include "Scope/Control/Commands/Command/CommandRunning.h"
#include "Scope/Control/IUnpacker.h"
#include "Scope/Control/ICommand.h"
#include "Scope/Core/IScope.h"

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandRunningParserPrivateData* CommandRunningParserHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/**
 * Constructor
 * @param scope
 * @param unpacker
 * @return
 */
CommandRunningParserHandle CommandRunningParser_create(IScopeHandle scope, IUnpackerHandle unpacker,
        IObserverHandle observer);

/**
 * Returns the command
 * @param self
 * @return
 */
ICommandHandle CommandRunningParser_getCommand(CommandRunningParserHandle self);

/**
 * Returns the name of the command
 * @return
 */
char* CommandRunningParser_getName(void);

/**
 * Deconstructor
 * @param self
 */
void CommandRunningParser_destroy(CommandRunningParserHandle self);

#endif
