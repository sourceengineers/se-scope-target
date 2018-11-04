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

#include <Scope/IScope.h>
#include <Scope/Command/ICommand.h>
#include <Scope/GeneralPurpose/DataTypes.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandTransPrivateData* CommandTransHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the command */
CommandTransHandle CommandTrans_create(IScopeHandle iScope);

/* Deconstructor: Deletes the instance of the command */
void CommandTrans_destroy(CommandTransHandle self);

/* Returns the command interface */
ICommandHandle CommandTrans_getICommand(CommandTransHandle self);

/* Returns the name of the command */
const char* CommandTrans_getName(CommandTransHandle self);


#endif
