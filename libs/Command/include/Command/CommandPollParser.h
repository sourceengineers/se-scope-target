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

#include <Command/ICommand.h>
#include <Communication/Receiver.h>
#include <Command/CommandPoll.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandPollParserPrivateData* CommandPollParserHandle;

/******************************************************************************
 Public functions
******************************************************************************/
/* Constructor: Creates a new instance of the command parser */
CommandPollParserHandle CommandPollParser_create(ICommandHandle iCommand, IUnpackerHandle iUnpacker);

/* Configurates the gives command */
void CommandPollParser_configure(CommandPollParserHandle self);

/* Deconstructor: Deletes the instance of the command parser */
void CommandPollParser_destroy(CommandPollParserHandle self);

#endif
