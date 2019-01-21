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

#include <Scope/IScope.h>
#include <Scope/Command/ICommand.h>
#include <Scope/GeneralPurpose/DataTypes.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandPollPrivateData* CommandPollHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the command */
CommandPollHandle CommandPoll_create(IScopeHandle scope);

/* Deconstructor: Deletes the instance of the command */
void CommandPoll_destroy(CommandPollHandle self);

/* Sets the Attributes of the command */
void CommandPoll_setAttributes(CommandPollHandle self, uint32_t timestamp);

/* Returns the command interface */
ICommandHandle CommandPoll_getICommand(CommandPollHandle self);

#endif
