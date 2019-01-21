/*!*****************************************************************************
 * @file         CommandAddr.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the cf_addr command
 *                
 *               To configure the command, the CommandAddrConf Object has to be 
 *               passed to the setCommandAttribute function.
 *                
 *               The field numberOfChangedChannels is indicating how many
 *               many channels will be changed. The field changedChannels 
 *               contains the channels which will be changed. The field 
 *               newAddresses contains the new addresses and types the machting 
 *               types of addresses to which will be fed into the channels.
 *
 *               All of these fields cannot be longer than 
 *               numberOfChangedChannels or it will be ingored.
 *              
 ******************************************************************************/

#ifndef COMMANDADDR_H_
#define COMMANDADDR_H_

#include <Scope/Core/IScope.h>
#include <Scope/Parser/Command/ICommand.h>
#include <Scope/GeneralPurpose/DataTypes.h>

/* Definition of the configuration struct */
typedef struct {
    void** newAddresses;          // Array to keep track on witch channels have changed
    uint32_t* changedChannels;         // Id's of the channels stored in "newAddresses"
    uint32_t numberOfChangedChannels;
    DATA_TYPES* types;            // Types of the channels
} CommandAddrConf;

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandAddrPrivateData* CommandAddrHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the command */
CommandAddrHandle CommandAddr_create(IScopeHandle scope);

/* Deconstructor: Deletes the instance of the command */
void CommandAddr_destroy(CommandAddrHandle self);

/* Sets the Attributes of the command */
void CommandAddr_setAttributes(CommandAddrHandle self, CommandAddrConf conf);

/* Returns the command interface */
ICommandHandle CommandAddr_getICommand(CommandAddrHandle self);

#endif
