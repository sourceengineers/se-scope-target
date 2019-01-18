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

#include <Scope/Communication/Receiver.h>
#include <Scope/Channel.h>
#include <Scope/Command/ICommand.h>
#include <Scope/GeneralPurpose/DataTypes.h>

/* Struct used to help the command configure the channels */
typedef struct {
    CHANNEL_STATES* newStates;
    int* changedChannels;
    int numberOfChangedChannels;
} CommandRunningConf;

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandRunningPrivateData* CommandRunningHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the command */
CommandRunningHandle CommandRunning_create(ChannelHandle* channels, size_t amountOfChannels);

/* Deconstructor: Deletes the instance of the command */
void CommandRunning_destroy(CommandRunningHandle self);

/* Returns the command interface */
ICommandHandle CommandRunning_getICommand(CommandRunningHandle self);

#endif
