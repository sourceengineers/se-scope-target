/*!*****************************************************************************
 * @file         CommandPackParserDispatcher.h
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

#ifndef COMMANDPACKPARSERDISPATCHER_H_
#define COMMANDPACKPARSERDISPATCHER_H_

#include "Scope/Control/AnnounceStorage.h"

#include "Scope/Control/PackCommands/CommandParser/CommandPackAnnounceParser.h"
#include "Scope/Control/PackCommands/CommandParser/CommandPackDataParser.h"
#include "Scope/Control/ICommand.h"
#include "Scope/Control/IPacker.h"
#include "Scope/Core/IScope.h"

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandPackParserDispatcherPrivateData* CommandPackParserDispatcherHandle;

/******************************************************************************
 Public functions 
******************************************************************************/

/**
 * Constructor
 * @param scope
 * @param announceStorage
 * @param packer
 * @return
 */
CommandPackParserDispatcherHandle
CommandPackParserDispatcher_create(IScopeHandle scope, AnnounceStorageHandle announceStorage, IPackerHandle packer);

/**
 * Returns the command with name matching command.
 * @param self
 * @param command
 * @return Returns NULL if command matches no name of a command
 */
ICommandHandle CommandPackParserDispatcher_run(CommandPackParserDispatcherHandle self, const char* command);

/**
 * Deconstructor
 * @param self
 */
void CommandPackParserDispatcher_destroy(CommandPackParserDispatcherHandle self);

#endif
