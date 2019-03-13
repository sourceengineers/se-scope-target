/*!*****************************************************************************
 * @file         CommandPackParserDispatcher.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Factory to generate the different Command interfaces 
 * 
 *               Since the usage of malloc after the initial construction is 
 *               not permitted, all objects returned by the commandParserDispatcher are pre-
 *               generated during the construction process.
 *
 ******************************************************************************/

#ifndef COMMANDPACKPARSERDISPATCHER_H_
#define COMMANDPACKPARSERDISPATCHER_H_

#include <Scope/Control/CommandParser/CommandPackAnnounceParser.h>
#include <Scope/Control/CommandParser/CommandPackDataParser.h>
#include <Scope/Control/Command/ICommand.h>
#include <Scope/Control/IPacker.h>
#include <Scope/Core/IScope.h>
#include <Scope/Control/AddressStorage.h>

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
 * @param addressStorage
 * @param packer
 * @return
 */
CommandPackParserDispatcherHandle
CommandPackParserDispatcher_create(IScopeHandle scope, AddressStorageHandle addressStorage, IPackerHandle packer);

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
