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

#include "Scope/GeneralPurpose/DataTypes.h"

#include "Scope/Control/ICommand.h"
#include "Scope/Core/IScope.h"
#include "Scope/Core/ScopeTypes.h"

#include <stdint.h>
#include <stdbool.h>

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
/**
 * Constructor
 * @param scope
 * @return
 */
CommandRunningHandle CommandRunning_create(IScopeHandle scope);

/**
 * Set the attributes which the command will use to configure the scope
 * @param self
 * @param conf
 */
void CommandRunning_setAttributes(CommandRunningHandle self, CommandRunningConf conf);

/**
 * Returns the ICommand interface
 * @param self
 * @return
 */
ICommandHandle CommandRunning_getICommand(CommandRunningHandle self);

/**
 * Deconstructor
 * @param self
 */
void CommandRunning_destroy(CommandRunningHandle self);

#endif
