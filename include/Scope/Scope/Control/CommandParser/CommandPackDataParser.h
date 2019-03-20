/*!*****************************************************************************
 * @file         CommandPackDataParser.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the configurator for the ev_pack.
 *
 *              
 ******************************************************************************/

#ifndef COMMANDPACKDATAPARSER_H_
#define COMMANDPACKDATAPARSER_H_

#include <Scope/Control/Command/CommandPackData.h>
#include <Scope/Control/Command/ICommand.h>
#include <Scope/Control/IPacker.h>
#include <Scope/Core/IScope.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandPackDataParserPrivateData* CommandPackDataParserHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/**
 * Constructor
 * @param scope
 * @param packer
 * @return
 */
CommandPackDataParserHandle CommandPackDataParser_create(IScopeHandle scope, IPackerHandle packer);

/**
 * Returns the command
 * @param self
 * @return
 */
ICommandHandle CommandPackDataParser_getCommand(CommandPackDataParserHandle self);

/**
 * Returns the name of the command
 * @return
 */
char* CommandPackDataParser_getName(void);

/**
 * Deconstructor
 * @param self
 */
void CommandPackDataParser_destroy(CommandPackDataParserHandle self);

#endif
