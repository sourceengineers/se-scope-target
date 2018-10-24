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

#include <Scope/Command/CommandRunning.h>
#include <Scope/Command/CommandPoll.h>
#include <Scope/Command/CommandAddr.h>
#include <Scope/Command/CommandTInc.h>
#include <Scope/Command/CommandTrans.h>
#include <Scope/Command/CommandTrigger.h>
#include <Scope/Command/CommandAddrParser.h>
#include <Scope/Command/CommandRunningParser.h>
#include <Scope/Command/CommandTIncParser.h>
#include <Scope/Command/CommandTriggerParser.h>
#include <Scope/Command/CommandAnnounce.h>
#include <Scope/Command/CommandPollParser.h>

#include <Scope/IScope.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandFactoryPrivateData* CommandFactoryHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the factory */
CommandFactoryHandle CommandFactory_create(IScopeHandle iScope, 
                                           ChannelHandle* channels, 
                                           size_t amountOfChannels,
                                           TriggerHandle trigger,
                                           IUnpackerHandle unpacker);
                                           
/* Deconstructor: Deletes the instance of the factory */
void CommandFactory_destroy(CommandFactoryHandle self);

/* Returns the command interface */
ICommandHandle CommandFactory_getICommand(CommandFactoryHandle self, const char* command);


#endif
