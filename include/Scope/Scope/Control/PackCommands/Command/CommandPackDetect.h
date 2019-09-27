/*!*****************************************************************************
 * @file         CommandPackDetect.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the ev_pack_announce command.
 *               Packs the announce data and triggers a transmission event.
 *
 ******************************************************************************/

#ifndef COMMANDPACKDETECT_H_
#define COMMANDPACKDETECT_H_

#include <Scope/Control/ICommand.h>
#include <Scope/Control/IPacker.h>
#include <Scope/Core/IScope.h>
#include <Scope/GeneralPurpose/DataTypes.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandPackDetectPrivateData* CommandPackDetectHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/**
 * Constructor
 * @param addressStorage
 * @param packer
 * @return
 */
CommandPackDetectHandle CommandPackDetect_create(IPackerHandle packer);

/**
 * Returns the ICommand interface
 * @param self
 * @return
 */
ICommandHandle CommandPackDetect_getICommand(CommandPackDetectHandle self);

/**
 * Deconstructor
 * @param self
 */
void CommandPackDetect_destroy(CommandPackDetectHandle self);

#endif
