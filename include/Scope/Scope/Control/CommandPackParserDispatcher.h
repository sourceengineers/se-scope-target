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

#include <Scope/Control/CommandParser/CommandPackDataParser.h>
#include <Scope/Control/CommandParser/CommandPackAnnounceParser.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandPackParserDispatcherPrivateData* CommandPackParserDispatcherHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the commandParserDispatcher */
CommandPackParserDispatcherHandle
CommandPackParserDispatcher_create(IScopeHandle scope, IPackerHandle packer);

/* Deconstructor: Deletes the instance of the commandParserDispatcher */
void CommandPackParserDispatcher_destroy(CommandPackParserDispatcherHandle self);

/* Returns the command interface */
ICommandHandle CommandPackParserDispatcher_run(CommandPackParserDispatcherHandle self, const char* command);


#endif
