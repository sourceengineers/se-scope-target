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

#include <Scope/Channel.h>
#include <Command/ICommand.h>

typedef struct {
  void** newAddresses;
  int* changedChannels;
  int numberOfChangedChannels;
  DATA_TYPES* types;
} CommandAddrConf;


/* Defines class */
typedef struct __CommandAddrPrivateData* CommandAddrHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instanze of the channel */
CommandAddrHandle CommandAddr_create(ChannelHandle* channels, const size_t ammountOfChannels);

/* Deconstructor: Deletes the instanze of the channel */
void CommandAddr_destroy(CommandAddrHandle self);

/* Returns the command interface */
ICommandHandle CommandAddr_getICommand(CommandAddrHandle self);

/* Returns the name of the command */
const char* CommandAddr_getName(CommandAddrHandle self);


#endif
