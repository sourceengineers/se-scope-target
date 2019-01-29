/*!*****************************************************************************
 * @file         CommandPack.h
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
 
#ifndef COMMANDPACK_H_
#define COMMANDPACK_H_

#include <Scope/Core/IScope.h>
#include <Scope/Control/Command/ICommand.h>
#include <Scope/GeneralPurpose/DataTypes.h>
#include <Scope/Control/IPacker.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandPackPrivateData* CommandPackHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the command */
CommandPackHandle CommandPack_create(IScopeHandle scope, IPackerHandle packer);

/* Deconstructor: Deletes the instance of the command */
void CommandPack_destroy(CommandPackHandle self);

/* Returns the command interface */
ICommandHandle CommandPack_getICommand(CommandPackHandle self);

#endif
