/*!*****************************************************************************
 * @file         CommandAnnounceParser.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @license      This file is part of SE-Scope.
 *
 *               SE-Scope is free software; you can redistribute it and/or
 *               modify it under the terms of the GNU General Public License as
 *               published by the Free Software Foundation.
 *
 *               SE-Scope is distributed in the hope that it will be useful,
 *               but WITHOUT ANY WARRANTY; without even the implied warranty of
 *               MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *               GNU General Public License for more details.
 *
 *               You should have received a copy of the GNU General Public License
 *               along with SE-Scope.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the configurator for the cf_announce.
 *
 *              
 ******************************************************************************/

#ifndef COMMANDANNOUNCEPARSER_H_
#define COMMANDANNOUNCEPARSER_H_

#include "Scope/Control/Commands/Command/CommandAnnounce.h"
#include "Scope/Control/ICommand.h"
#include "Scope/Core/IScope.h"
#include <se-lib-c/util/observer/IObserver.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandAnnounceParserPrivateData* CommandAnnounceParserHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/**
 * Constructor
 * @param packObserver
 * @return
 */
CommandAnnounceParserHandle CommandAnnounceParser_create(IObserverHandle packObserver);

/**
 * Returns the command
 * @param self
 * @return
 */
ICommandHandle CommandAnnounceParser_getCommand(CommandAnnounceParserHandle self);

/**
 * Deconstructor
 * @param self
 */
 void CommandAnnounceParser_destroy(CommandAnnounceParserHandle self);

#endif
