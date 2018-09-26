/*!*****************************************************************************
 * @file         CommandAddr.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the cf_addr command
 *                
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
