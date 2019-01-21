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

#include <Scope/Core/IScope.h>
#include <Scope/Parser/Command/ICommand.h>
#include <Scope/GeneralPurpose/DataTypes.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandClearPrivateData* CommandClearHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the command */
CommandClearHandle CommandClear_create(IScopeHandle scope);

/* Deconstructor: Deletes the instance of the command */
void CommandClear_destroy(CommandClearHandle self);

/* Returns the command interface */
ICommandHandle CommandClear_getICommand(CommandClearHandle self);



#endif
