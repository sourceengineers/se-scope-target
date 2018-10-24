/*!*****************************************************************************
 * @file         CommandAnnounce.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the ev_announce command
 *                
 *               Starts a sending sending the pending addresses
 * 
 ******************************************************************************/
 
#ifndef COMMANDANNOUNCE_H_
#define COMMANDANNOUNCE_H_

#include <Scope/IScope.h>
#include <Scope/Command/ICommand.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandAnnouncePrivateData* CommandAnnounceHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the command */
CommandAnnounceHandle CommandAnnounce_create(IScopeHandle iScope);

/* Deconstructor: Deletes the instance of the command */
void CommandAnnounce_destroy(CommandAnnounceHandle self);

/* Returns the command interface */
ICommandHandle CommandAnnounce_getICommand(CommandAnnounceHandle self);

/* Returns the name of the command */
const char* CommandAnnounce_getName(CommandAnnounceHandle self);


#endif
