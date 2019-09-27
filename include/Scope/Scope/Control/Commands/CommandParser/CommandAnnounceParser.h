/*!*****************************************************************************
 * @file         CommandAnnounceParser.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the configurator for the cf_announce.
 *
 *              
 ******************************************************************************/

#ifndef COMMANDANNOUNCEPARSER_H_
#define COMMANDANNOUNCEPARSER_H_

#include <Scope/Control/Commands/Command/CommandAnnounce.h>
#include <Scope/Control/ICommand.h>
#include <Scope/Core/IScope.h>
#include <Scope/GeneralPurpose/IObserver.h>

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
 * Returns the name of the command
 * @return
 */
char* CommandAnnounceParser_getName(void);


/**
 * Deconstructor
 * @param self
 */
 void CommandAnnounceParser_destroy(CommandAnnounceParserHandle self);

#endif
