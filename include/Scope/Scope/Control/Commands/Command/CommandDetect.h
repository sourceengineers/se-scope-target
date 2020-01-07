/*!*****************************************************************************
 * @file         CommandDetect.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the ev_Detect command
 *                
 *               Starts a sending sending the pending addresses
 * 
 ******************************************************************************/

#ifndef COMMANDDETECT_H_
#define COMMANDDETECT_H_

#include "Scope/GeneralPurpose/DataTypes.h"

#include "Scope/Control/ICommand.h"
#include "Scope/Core/IScope.h"
#include <se-lib-c/util/observer/IObserver.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandDetectPrivateData* CommandDetectHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/**
 * Constructor
 * @param packObserver Dependency to the pack observer which will be updated during the execution of the command
 * @return
 */
CommandDetectHandle CommandDetect_create(IObserverHandle packObserver);

/**
 * Returns the ICommand interface
 * @param self
 * @return
 */
ICommandHandle CommandDetect_getICommand(CommandDetectHandle self);

/**
 * Deconstructor
 * @param self
 */
void CommandDetect_destroy(CommandDetectHandle self);
#endif
