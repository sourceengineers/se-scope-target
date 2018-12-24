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

#include <Scope/IScope.h>
#include <Scope/Command/ICommand.h>
#include <Scope/GeneralPurpose/DataTypes.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandTIncPrivateData* CommandTIncHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the command */
CommandTIncHandle CommandTInc_create(IScopeHandle scope);

/* Deconstructor: Deletes the instance of the command */
void CommandTInc_destroy(CommandTIncHandle self);

/* Returns the command interface */
ICommandHandle CommandTInc_getICommand(CommandTIncHandle self);

/* Returns the name of the command */
char* CommandTInc_getName(CommandTIncHandle self);


#endif
