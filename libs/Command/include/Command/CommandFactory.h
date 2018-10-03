/*!*****************************************************************************
 * @file         CommandFactory.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Factory to generate the different Command interfaces 
 * 
 *               Since the usage of malloc after the initial construction is 
 *               not permitted, all objects returned by the factory are pre- 
 *               generated during the construction process.
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
#include <Command/CommandAddrParser.h>
#include <Command/CommandRunningParser.h>
#include <Command/CommandTIncParser.h>
#include <Command/CommandTriggerParser.h>

#include <Scope/Scope.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandFactoryPrivateData* CommandFactoryHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instanze of the factory */
CommandFactoryHandle CommandFactory_create(IScopeHandle iScope, 
                                           ChannelHandle* channels, 
                                           size_t ammountOfChannels,
                                           TriggerHandle trigger,
                                           IUnpackerHandle unpacker);
                                           
/* Deconstructor: Deletes the instanze of the factory */
void CommandFactory_destroy(CommandFactoryHandle self);

/* Returns the command interface */
ICommandHandle CommandFactory_getICommand(CommandFactoryHandle self, const char* command);


#endif
