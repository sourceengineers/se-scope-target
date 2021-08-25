/*!*****************************************************************************
 * @file         CommandTrigger.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the cf_trigger command
 *                
 *               Configurates the Trigger. The used structure is defined and 
 *               explained in the trigger.h file.
 ******************************************************************************/

#ifndef COMMANDTRIGGER_H_
#define COMMANDTRIGGER_H_

#include <se-lib-c/util/observer/IObserver.h>
#include "Scope/GeneralPurpose/DataTypes.h"

#include "Scope/Control/ICommand.h"
#include "Scope/Core/IScope.h"
#include "Scope/Core/ScopeTypes.h"

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandTriggerPrivateData* CommandTriggerHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/**
 * Constructor
 * @param scope
 * @return
 */
CommandTriggerHandle CommandTrigger_create(IScopeHandle scope, IObserverHandle observer);

/**
 * Set the attributes which the command will use to configure the scope
 * @param self
 * @param conf
 */
void CommandTrigger_setAttributes(CommandTriggerHandle self, TriggerConfiguration conf);

/**
 * Returns the ICommand interface
 * @param self
 * @return
 */
ICommandHandle CommandTrigger_getICommand(CommandTriggerHandle self);

/**
 * Deconstructor
 * @param self
 */
void CommandTrigger_destroy(CommandTriggerHandle self);

#endif
