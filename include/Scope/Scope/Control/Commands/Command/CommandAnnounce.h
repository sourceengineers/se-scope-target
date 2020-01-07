/*!*****************************************************************************
 * @file         CommandAnnounce.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the ev_announce command
 *                
 *               Starts a sending sending the pending addresses
 * 
 ******************************************************************************/

#ifndef COMMANDANNOUNCE_H_
#define COMMANDANNOUNCE_H_

#include <se-lib-c/util/observer/IObserver.h>

#include "Scope/GeneralPurpose/DataTypes.h"
#include "Scope/Control/ICommand.h"
#include "Scope/Core/IScope.h"


/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandAnnouncePrivateData* CommandAnnounceHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/**
 * Constructor
 * @param packObserver Dependency to the pack observer which will be updated during the execution of the command
 * @return
 */
CommandAnnounceHandle CommandAnnounce_create(IObserverHandle packObserver);

/**
 * Returns the ICommand interface
 * @param self
 * @return
 */
ICommandHandle CommandAnnounce_getICommand(CommandAnnounceHandle self);

/**
 * Deconstructor
 * @param self
 */
void CommandAnnounce_destroy(CommandAnnounceHandle self);
#endif
