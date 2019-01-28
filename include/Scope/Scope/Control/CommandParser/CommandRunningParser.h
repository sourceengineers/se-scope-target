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

#include <Scope/Control/Command/ICommand.h>
#include <Scope/Control/Command/CommandRunning.h>
#include <Scope/GeneralPurpose/DataTypes.h>
#include <Scope/Control/IUnpacker.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandRunningParserPrivateData* CommandRunningParserHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the command parser */
CommandRunningParserHandle CommandRunningParser_create(IScopeHandle scope, IUnpackerHandle unpacker);

/* Configurates the gives command */
ICommandHandle CommandRunningParser_getCommand(CommandRunningParserHandle self);

/* Returns the name of the command */
char* CommandRunningParser_getName();

/* Deconstructor: Deletes the instance of the command parser */
void CommandRunningParser_destroy(CommandRunningParserHandle self);

#endif
