/*!*****************************************************************************
 * @file         CommandParser.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Factory to generate the different Command interfaces 
 * 
 *               Since the usage of malloc after the initial construction is 
 *               not permitted, all objects returned by the dispatcher are pre-
 *               generated during the construction process.
 *
 ******************************************************************************/ 
 
#ifndef COMMANDFACTORY_H_
#define COMMANDFACTORY_H_

#include <Scope/Command/CommandAddrParser.h>
#include <Scope/Command/CommandRunningParser.h>
#include <Scope/Command/CommandTIncParser.h>
#include <Scope/Command/CommandTriggerParser.h>
#include <Scope/Command/CommandPollParser.h>
#include <Scope/Command/CommandAnnounceParser.h>
#include <Scope/Command/CommandTransParser.h>
#include <Scope/Command/CommandClearParser.h>

#include <Scope/IScope.h>
#include <Scope/GeneralPurpose/DataTypes.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandParserPrivateData* CommandParserHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the dispatcher */
CommandParserHandle CommandParser_create(IScopeHandle scope,
                                           ChannelHandle* channels, 
                                           size_t amountOfChannels,
                                           TriggerHandle trigger,
                                           IUnpackerHandle unpacker);
                                           
/* Deconstructor: Deletes the instance of the dispatcher */
void CommandParser_destroy(CommandParserHandle self);

/* Returns the command interface */
ICommandHandle CommandParser_getICommand(CommandParserHandle self, const char* command);


#endif
