/*!*****************************************************************************
 * @file         CommandTransParser.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the configurator for the ev_trans.
 *
 *              
 ******************************************************************************/

#ifndef COMMANDTRANSPARSER_H_
#define COMMANDTRANSPARSER_H_

#include <Scope/Control/Commands/Command/CommandTrans.h>
#include <Scope/Control/ICommand.h>
#include <Scope/Core/IScope.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandTransParserPrivateData* CommandTransParserHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/**
 * Constructor
 * @param scope
 * @return
 */
CommandTransParserHandle CommandTransParser_create(IScopeHandle scope);

/**
 * Returns the command
 * @param self
 * @return
 */
ICommandHandle CommandTransParser_getCommand(CommandTransParserHandle self);

/**
 * Returns the name of the command
 * @return
 */
char* CommandTransParser_getName(void);

/**
 * Deconstructor
 * @param self
 */
void CommandTransParser_destroy(CommandTransParserHandle self);

#endif
