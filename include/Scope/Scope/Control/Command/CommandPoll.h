/*!*****************************************************************************
 * @file         CommandPoll.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the ev_poll command
 *                
 *               Executes a Poll event. The function will be provided by the 
 *               IScope interface, to prevent code dublication.
 *
 ******************************************************************************/

#ifndef COMMANDPOLL_H_
#define COMMANDPOLL_H_

#include <Scope/Control/Command/ICommand.h>
#include <Scope/Core/IScope.h>
#include <Scope/GeneralPurpose/DataTypes.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandPollPrivateData* CommandPollHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/**
 * Constructor
 * @param scope
 * @return
 */
CommandPollHandle CommandPoll_create(IScopeHandle scope);

/**
 * Returns the ICommand interface
 * @param self
 * @return
 */
ICommandHandle CommandPoll_getICommand(CommandPollHandle self);

/**
 * Deconstructor
 * @param self
 */
void CommandPoll_destroy(CommandPollHandle self);

#endif
