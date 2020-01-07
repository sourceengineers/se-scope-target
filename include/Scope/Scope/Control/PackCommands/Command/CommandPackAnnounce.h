/*!*****************************************************************************
 * @file         CommandPackAnnounce.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the ev_pack_announce command.
 *               Packs the announce data and triggers a transmission event.
 *
 ******************************************************************************/

#ifndef COMMANDPACKANNOUNCE_H_
#define COMMANDPACKANNOUNCE_H_

#include <se-lib-c/util/observer/IObserver.h>
#include "Scope/GeneralPurpose/DataTypes.h"
#include "Scope/Control/AnnounceStorage.h"

#include "Scope/Control/ICommand.h"
#include "Scope/Control/IPacker.h"
#include "Scope/Core/IScope.h"

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandPackAnnouncePrivateData* CommandPackAnnounceHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/**
 * Constructor
 * @param announceStorage
 * @param packer
 * @return
 */
CommandPackAnnounceHandle CommandPackAnnounce_create(AnnounceStorageHandle announceStorage, IPackerHandle packer);

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
