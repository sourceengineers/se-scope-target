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
#include <Command/ICommand.h>


/* Defines class */
typedef struct __CommandPollPrivateData* CommandPollHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instanze of the channel */
CommandPollHandle CommandPoll_create(IScopeHandle iScope);

/* Deconstructor: Deletes the instanze of the channel */
void CommandPoll_destroy(CommandPollHandle self);

/* Returns the command interface */
ICommandHandle CommandPoll_getICommand(CommandPollHandle self);

/* Returns the name of the command */
const char* CommandPoll_getName(CommandPollHandle self);


#endif
