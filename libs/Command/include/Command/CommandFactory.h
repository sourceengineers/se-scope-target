/*!*****************************************************************************
 * @file         CommandFactory.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Factory to generate the different Command interfaces 
 * 
 ******************************************************************************/ 
 
#ifndef COMMANDFACTORY_H_
#define COMMANDFACTORY_H_

#include <Command/CommandRunning.h>
#include <Command/CommandPoll.h>
#include <Command/CommandAddr.h>
#include <Command/CommandTInc.h>
#include <Command/CommandTrans.h>
#include <Command/CommandTrigger.h>
#include <Scope/Scope.h>

/* Defines class */
typedef struct __CommandFactoryPrivateData* CommandFactoryHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instanze of the factory */
CommandFactoryHandle CommandFactory_create(IScopeHandle iScope, 
                                           ChannelHandle* channels, 
                                           size_t ammountOfChannels,
                                           TriggerHandle trigger);
                                           
/* Deconstructor: Deletes the instanze of the factory */
void CommandFactory_destroy(CommandFactoryHandle self);

/* Returns the command interface */
ICommandHandle CommandFactory_getICommand(CommandFactoryHandle self, const char* command);


#endif
