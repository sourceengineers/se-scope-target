/*!*****************************************************************************
 * @file         CommandTInc.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the cf_t_inc command
 *                
 *               Sets the time increment of the scope poll events. 
 *               The function will be provided by the IScope interface, to 
 *               prevent code dublication.
 *                
 ******************************************************************************/

#ifndef COMMANDTINC_H_
#define COMMANDTINC_H_

#include "Scope/GeneralPurpose/DataTypes.h"

#include "Scope/Control/ICommand.h"
#include "Scope/Core/IScope.h"

#include <stdint.h>
#include "Scope/GeneralPurpose/IObserver.h"

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandTIncPrivateData* CommandTIncHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/**
 * Constructor
 * @param scope
 * @return
 */
CommandTIncHandle CommandTInc_create(IScopeHandle scope, IObserverHandle observer);

/**
 * Set the attributes which the command will use to configure the scope
 * @param self
 * @param timeIncrement
 */
void CommandTInc_setAttributes(CommandTIncHandle self, uint32_t timeIncrement);

/**
 * Returns the ICommand interface
 * @param self
 * @return
 */
ICommandHandle CommandTInc_getICommand(CommandTIncHandle self);

/**
 * Deconstructor
 * @param self
 */
void CommandTInc_destroy(CommandTIncHandle self);

#endif
