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

#include <Scope/Control/Command/CommandClear.h>
#include <Scope/Control/Command/ICommand.h>
#include <Scope/Core/IScope.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandClearParserPrivateData* CommandClearParserHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the command parser */
CommandClearParserHandle CommandClearParser_create(IScopeHandle scope);

/* Configurates the gives command */
ICommandHandle CommandClearParser_getCommand(CommandClearParserHandle self);

/* Returns the name of the command */
char* CommandClearParser_getName(void);

/* Deconstructor: Deletes the instance of the command parser */
void CommandClearParser_destroy(CommandClearParserHandle self);

#endif
