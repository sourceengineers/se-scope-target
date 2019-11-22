/*!*****************************************************************************
 * @file         CommandClear.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the ev_clear command
 *                
 *               Starts a sending sending the pending addresses
 * 
 ******************************************************************************/

#ifndef COMMANDCLEAR_H_
#define COMMANDCLEAR_H_

#include "Scope/GeneralPurpose/DataTypes.h"

#include "Scope/Control/ICommand.h"
#include "Scope/Core/IScope.h"

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandClearPrivateData* CommandClearHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/**
 * Constructor
 * @param scope Dependency to the scope interface
 * @return
 */
 CommandClearHandle CommandClear_create(IScopeHandle scope);

/**
 * Returns the ICommand interface
 * @param self
 * @return
 */
 ICommandHandle CommandClear_getICommand(CommandClearHandle self);

/**
 * Deconstructor
 * @param self
 */
void CommandClear_destroy(CommandClearHandle self);


#endif
