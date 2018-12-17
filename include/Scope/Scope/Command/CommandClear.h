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

#include <Scope/IScope.h>
#include <Scope/Command/ICommand.h>
#include <Scope/GeneralPurpose/DataTypes.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandClearPrivateData* CommandClearHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the command */
CommandClearHandle CommandClear_create(IScopeHandle iScope);

/* Deconstructor: Deletes the instance of the command */
void CommandClear_destroy(CommandClearHandle self);

/* Returns the command interface */
ICommandHandle CommandClear_getICommand(CommandClearHandle self);

/* Returns the name of the command */
const char* CommandClear_getName(CommandClearHandle self);


#endif
