/*!*****************************************************************************
 * @file         CommandPackAnnounce.h
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

#ifndef COMMANDPACKANNOUNCE_H_
#define COMMANDPACKANNOUNCE_H_

#include <Scope/Control/Command/ICommand.h>
#include <Scope/Control/IPacker.h>
#include <Scope/Core/IScope.h>
#include <Scope/GeneralPurpose/DataTypes.h>
#include <Scope/Control/AddressStorage.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandPackAnnouncePrivateData* CommandPackAnnounceHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/**
 * Constructor
 * @param addressStorage
 * @param packer
 * @return
 */
CommandPackAnnounceHandle CommandPackAnnounce_create(AddressStorageHandle addressStorage, IPackerHandle packer);

/**
 * Returns the ICommand interface
 * @param self
 * @return
 */
ICommandHandle CommandPackAnnounce_getICommand(CommandPackAnnounceHandle self);

/**
 * Deconstructor
 * @param self
 */
void CommandPackAnnounce_destroy(CommandPackAnnounceHandle self);

#endif
