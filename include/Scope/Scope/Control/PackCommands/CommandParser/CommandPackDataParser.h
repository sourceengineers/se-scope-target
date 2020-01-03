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

#include "Scope/GeneralPurpose/IObserver.h"
#include "Scope/Control/PackCommands/Command/CommandPackData.h"
#include "Scope/Control/ICommand.h"
#include "Scope/Control/IPacker.h"
#include "Scope/Core/IScope.h"

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
 * Deconstructor
 * @param self
 */
void CommandPackDataParser_destroy(CommandPackDataParserHandle self);

#endif
