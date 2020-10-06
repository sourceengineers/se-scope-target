/*!*****************************************************************************
 * @file         CommandPackLog.h
 *
 * @copyright    Copyright (c) 2020 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Anselm Fuhrer anselm.fuhrer@sourceengineers.com
 *
 * @brief        Implementation of the ev_pack_log command
 *               Packs the log of the scope and triggers a transmit event.
 *
 ******************************************************************************/

#ifndef COMMANDPACKLOG_H_
#define COMMANDPACKLOG_H_

#include <se-lib-c/util/observer/IObserver.h>
#include "Scope/GeneralPurpose/DataTypes.h"

#include "Scope/Control/ICommand.h"
#include "Scope/Control/IPacker.h"
#include "Scope/Core/IScope.h"

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandPackLogPrivateData* CommandPackLogHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/**
 * Constructor
 * @param scope
 * @param packer
 * @return
 */
CommandPackLogHandle CommandPackLog_create(ScLogDataDef scope, IPackerHandle packer);

/**
 * Returns the ICommand interface
 * @param self
 * @return
 */
ICommandHandle CommandPackLog_getICommand(CommandPackLogHandle self);

/**
 * Deconstructor
 * @param self
 */
void CommandPackLog_destroy(CommandPackLogHandle self);

#endif
