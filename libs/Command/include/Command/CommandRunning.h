/*!*****************************************************************************
 * @file         CommandRunning.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the running command.
 *               
 *               As Attribute interface, the CommandRunningConfig struct is used
 *               If the channel state has changed, it has to be flagged in the 
 *               channelHasChanged array. 
 *               If the channels were flagged, their state will be changed when 
 *               the run() function is executed, according to the setRunning 
 *               array.
 *               Only CHANNEL_RUNNING and CHANNEL_STOPPED will affect the
 *               channel state.
 ******************************************************************************/
 
#ifndef COMMANDRUNNING_H_
#define COMMANDRUNNING_H_

#include <Communication/Parser.h>
#include <Scope/Channel.h>
#include <Command/ICommand.h>

CHANNEL_STATES* setRunning;

/* Defines class */
typedef struct __CommandRunningPrivateData* CommandRunningHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instanze of the channel */
CommandRunningHandle CommandRunning_create(ChannelHandle* channels, size_t ammountOfChannels);

/* Deconstructor: Deletes the instanze of the channel */
void CommandRunning_destroy(CommandRunningHandle self);

/* Returns the command interface */
ICommandHandle CommandRunning_getICommand(CommandRunningHandle self);

/* Returns the name of the command */
const char* CommandRunning_getName(CommandRunningHandle self);


#endif
