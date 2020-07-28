/*!*****************************************************************************
 * @file         CommandLog.h
 *
 * @copyright    Copyright (c) 2020 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Anselm Fuhrer anselm.fuhrer@sourceengineers.com
 *
 * @brief        TODO
 * 
 ******************************************************************************/

#ifndef COMMANDLOG_H_
#define COMMANDLOG_H_

#include <se-lib-c/util/observer/IObserver.h>

#include "Scope/Control/ICommand.h"
#include "Scope/Core/IScope.h"


/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandLogPrivateData* CommandLogHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/**
 * Constructor
 * @param packObserver Dependency to the pack observer which will be updated during the execution of the command
 * @return
 */
CommandLogHandle CommandLog_create(IObserverHandle packObserver);

/**
 * Returns the ICommand interface
 * @param self
 * @return
 */
ICommandHandle CommandLog_getICommand(CommandLogHandle self);

/**
 * Deconstructor
 * @param self
 */
void CommandLog_destroy(CommandLogHandle self);
#endif
