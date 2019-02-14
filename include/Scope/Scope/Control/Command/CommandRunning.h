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

#include <Scope/Core/IScope.h>
#include <Scope/Control/Command/ICommand.h>
#include <Scope/GeneralPurpose/DataTypes.h>

/* Struct used to help the command configure the channels */
typedef struct{
    CHANNEL_STATES* newStates;
    uint32_t* changedChannels;
    uint32_t numberOfChangedChannels;
    bool triggerChanged;
    bool triggerRunning;
} CommandRunningConf;

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandRunningPrivateData* CommandRunningHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the command */
CommandRunningHandle CommandRunning_create(IScopeHandle scope);

/* Deconstructor: Deletes the instance of the command */
void CommandRunning_destroy(CommandRunningHandle self);

/* Sets the Attributes of the command */
void CommandRunning_setAttributes(CommandRunningHandle self, CommandRunningConf conf);

/* Returns the command interface */
ICommandHandle CommandRunning_getICommand(CommandRunningHandle self);

#endif