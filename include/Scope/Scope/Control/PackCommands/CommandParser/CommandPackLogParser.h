/*!*****************************************************************************
 * @file         CommandPackLogParser.h
 *
 * @copyright    Copyright (c) 2020 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Anselm Fuhrer anselm.fuhrer@sourceengineers.com
 *
 * @brief        Implementation of the configurator for the ev_pack.
 *
 *
 ******************************************************************************/

#ifndef COMMANDPACKLOGPARSER_H_
#define COMMANDPACKLOGPARSER_H_

#include <se-lib-c/util/observer/IObserver.h>
#include "Scope/Control/PackCommands/Command/CommandPackLog.h"
#include "Scope/Control/ICommand.h"
#include "Scope/Control/IPacker.h"
#include "Scope/Core/IScope.h"

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandPackLogParserPrivateData* CommandPackLogParserHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/**
 * Constructor
 * @param scope
 * @param packer
 * @return
 */
CommandPackLogParserHandle CommandPackLogParser_create(ScLogDataDef log, IPackerHandle packer);

/**
 * Returns the command
 * @param self
 * @return
 */
ICommandHandle CommandPackLogParser_getCommand(CommandPackLogParserHandle self);

/**
 * Deconstructor
 * @param self
 */
void CommandPackLogParser_destroy(CommandPackLogParserHandle self);

#endif
