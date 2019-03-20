/*!*****************************************************************************
 * @file         CommandTInc.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the configurator for the cf_TInc.
 *
 *              
 ******************************************************************************/

#ifndef COMMANDTINCPARSER_H_
#define COMMANDTINCPARSER_H_

#include <Scope/Control/Command/CommandTInc.h>
#include <Scope/Control/IUnpacker.h>
#include <Scope/Control/Command/ICommand.h>
#include <Scope/Core/IScope.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandTIncParserPrivateData* CommandTIncParserHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/**
 * Constructor
 * @param scope
 * @param unpacker
 * @return
 */
CommandTIncParserHandle CommandTIncParser_create(IScopeHandle scope, IUnpackerHandle unpacker);

/**
 * Returns the command
 * @param self
 * @return
 */
ICommandHandle CommandTIncParser_getCommand(CommandTIncParserHandle self);

/**
 * Returns the name of the command
 * @return
 */
char* CommandTIncParser_getName(void);

/**
 * Deconstructor
 * @param self
 */
void CommandTIncParser_destroy(CommandTIncParserHandle self);

#endif
