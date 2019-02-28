/*!*****************************************************************************
 * @file         CommandPackData.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the ev_pack command
 *                
 *               Starts a sending event, provided by the IScope interface
 * 
 ******************************************************************************/

#ifndef COMMANDPACKDATA_H_
#define COMMANDPACKDATA_H_

#include <Scope/Core/IScope.h>
#include <Scope/Control/Command/ICommand.h>
#include <Scope/GeneralPurpose/DataTypes.h>
#include <Scope/Control/IPacker.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandPackDataPrivateData* CommandPackDataHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the command */
CommandPackDataHandle CommandPackData_create(IScopeHandle scope, IPackerHandle packer);

/* Deconstructor: Deletes the instance of the command */
void CommandPackData_destroy(CommandPackDataHandle self);

/* Returns the command interface */
ICommandHandle CommandPackData_getICommand(CommandPackDataHandle self);

#endif
