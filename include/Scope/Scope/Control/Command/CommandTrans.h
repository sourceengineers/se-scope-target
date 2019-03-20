/*!*****************************************************************************
 * @file         CommandTrans.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the ev_trans command
 *                
 *               Starts a sending event, provided by the IScope interface
 * 
 ******************************************************************************/

#ifndef COMMANDTRANS_H_
#define COMMANDTRANS_H_

#include <Scope/Control/Command/ICommand.h>
#include <Scope/Core/IScope.h>
#include <Scope/GeneralPurpose/DataTypes.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandTransPrivateData* CommandTransHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/**
 * Constructor
 * @param scope
 * @return
 */
CommandTransHandle CommandTrans_create(IScopeHandle scope);

/**
 * Returns the ICommand interface
 * @param self
 * @return
 */
ICommandHandle CommandTrans_getICommand(CommandTransHandle self);

/**
 * Deconstructor
 * @param self
 */
void CommandTrans_destroy(CommandTransHandle self);

#endif
