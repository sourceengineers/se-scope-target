/*!*****************************************************************************
 * @file         CommandPackData.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the ev_pack_data command
 *               Packs the data of the scope and triggers a transmit event.
 *
 ******************************************************************************/

#ifndef COMMANDPACKDATA_H_
#define COMMANDPACKDATA_H_

#include <Scope/Control/ICommand.h>
#include <Scope/Control/IPacker.h>
#include <Scope/Core/IScope.h>
#include <Scope/GeneralPurpose/DataTypes.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandPackDataPrivateData* CommandPackDataHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/**
 * Constructor
 * @param scope
 * @param packer
 * @return
 */
CommandPackDataHandle CommandPackData_create(IScopeHandle scope, IPackerHandle packer);

/**
 * Returns the ICommand interface
 * @param self
 * @return
 */
ICommandHandle CommandPackData_getICommand(CommandPackDataHandle self);

/**
 * Deconstructor
 * @param self
 */
void CommandPackData_destroy(CommandPackDataHandle self);

#endif
