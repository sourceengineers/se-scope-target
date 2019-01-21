/*!*****************************************************************************
 * @file         CommandPollParser.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the configurator for the ev_clear.
 *
 *
 ******************************************************************************/

#ifndef COMMANDPOLLPARSER_H_
#define COMMANDPOLLPARSER_H_

#include <Scope/Parser/Command/ICommand.h>
#include <Scope/Communication/Receiver.h>
#include <Scope/Parser/Command/CommandPoll.h>
#include <Scope/GeneralPurpose/DataTypes.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandPollParserPrivateData* CommandPollParserHandle;

/******************************************************************************
 Public functions
******************************************************************************/
/* Constructor: Creates a new instance of the command parser */
CommandPollParserHandle CommandPollParser_create(IScopeHandle scope, IUnpackerHandle unpacker);

/* Configurates the gives command */
ICommandHandle CommandPollParser_getCommand(CommandPollParserHandle self);

/* Returns the name of the command */
char* CommandPollParser_getName();

/* Deconstructor: Deletes the instance of the command parser */
void CommandPollParser_destroy(CommandPollParserHandle self);

#endif
