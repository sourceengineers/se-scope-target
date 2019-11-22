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

#include "Scope/Control/Commands/Command/CommandPoll.h"
#include "Scope/Control/ICommand.h"
#include "Scope/Core/IScope.h"

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandPollParserPrivateData* CommandPollParserHandle;

/******************************************************************************
 Public functions
******************************************************************************/
/**
 * Constructor
 * @param scope
 * @return
 */
CommandPollParserHandle CommandPollParser_create(IScopeHandle scope);

/**
 * Returns the command
 * @param self
 * @return
 */
ICommandHandle CommandPollParser_getCommand(CommandPollParserHandle self);

/**
 * Returns the name of the command
 * @return
 */
char* CommandPollParser_getName(void);

/**
 * Deconstructor
 * @param self
 */
void CommandPollParser_destroy(CommandPollParserHandle self);

#endif
