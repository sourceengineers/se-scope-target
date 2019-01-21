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

#include <Scope/Core/IScope.h>
#include <Scope/Parser/Command/ICommand.h>
#include <Scope/GeneralPurpose/DataTypes.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandTransPrivateData* CommandTransHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the command */
CommandTransHandle CommandTrans_create(IScopeHandle scope);

/* Deconstructor: Deletes the instance of the command */
void CommandTrans_destroy(CommandTransHandle self);

/* Returns the command interface */
ICommandHandle CommandTrans_getICommand(CommandTransHandle self);

#endif
