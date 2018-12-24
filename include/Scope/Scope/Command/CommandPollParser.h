/*!*****************************************************************************
 * @file         CommandPoll.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the configurator for the cf_poll.
 *
 *
 ******************************************************************************/

#ifndef COMMANDPOLLPARSER_H_
#define COMMANDPOLLPARSER_H_

#include <Scope/Command/ICommand.h>
#include <Scope/Communication/Receiver.h>
#include <Scope/Command/CommandPoll.h>
#include <Scope/GeneralPurpose/DataTypes.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandPollParserPrivateData* CommandPollParserHandle;

/******************************************************************************
 Public functions
******************************************************************************/
/* Constructor: Creates a new instance of the command parser */
CommandPollParserHandle CommandPollParser_create(ICommandHandle command, IUnpackerHandle unpacker);

/* Configurates the gives command */
void CommandPollParser_configure(CommandPollParserHandle self);

/* Deconstructor: Deletes the instance of the command parser */
void CommandPollParser_destroy(CommandPollParserHandle self);

#endif
