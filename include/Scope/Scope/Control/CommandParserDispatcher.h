/*!*****************************************************************************
 * @file         CommandParserDispatcher.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Factory to generate the different command interfaces
 * 
 *               Since the usage of malloc after the initial construction is 
 *               not permitted, all objects returned by the commandParserDispatcher are pre-
 *               generated during the construction process.
 *
 ******************************************************************************/

#ifndef COMMANDPARSERDISPATCHER_H_
#define COMMANDPARSERDISPATCHER_H_

#include <Scope/Control/Commands/CommandParser/CommandAddrParser.h>
#include <Scope/Control/Commands/CommandParser/CommandAnnounceParser.h>
#include <Scope/Control/Commands/CommandParser/CommandClearParser.h>
#include <Scope/Control/Commands/CommandParser/CommandPollParser.h>
#include <Scope/Control/Commands/CommandParser/CommandRunningParser.h>
#include <Scope/Control/Commands/CommandParser/CommandTIncParser.h>
#include <Scope/Control/Commands/CommandParser/CommandTransParser.h>
#include <Scope/Control/Commands/CommandParser/CommandTriggerParser.h>
#include <Scope/Control/ICommand.h>
#include <Scope/Control/IUnpacker.h>
#include <Scope/Core/IScope.h>
#include <Scope/GeneralPurpose/IObserver.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandParserDispatcherPrivateData* CommandParserDispatcherHandle;

/******************************************************************************
 Public functions 
******************************************************************************/

/**
 * Constructor
 * @param scope
 * @param packObserver
 * @param unpacker
 * @return
 */
CommandParserDispatcherHandle
CommandParserDispatcher_create(IScopeHandle scope, IObserverHandle packObserver, IUnpackerHandle unpacker);

/**
 * Returns the command with name matching command.
 * @param self
 * @param command
 * @return Returns NULL if command matches no name of a command
 */
ICommandHandle CommandParserDispatcher_run(CommandParserDispatcherHandle self, const char* command);

/**
 * Deconstructor
 * @param self
 */
void CommandParserDispatcher_destroy(CommandParserDispatcherHandle self);

#endif
