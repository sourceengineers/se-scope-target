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

#include <Scope/Control/Command/ICommand.h>
#include <Scope/Communication/Receiver.h>
#include <Scope/Control/Command/CommandAnnounce.h>
#include <Scope/GeneralPurpose/DataTypes.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandAnnounceParserPrivateData* CommandAnnounceParserHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the command parser */
CommandAnnounceParserHandle CommandAnnounceParser_create(IScopeHandle scope);

/* Configurates the gives command */
ICommandHandle CommandAnnounceParser_getCommand(CommandAnnounceParserHandle self);

/* Returns the name of the command */
char* CommandAnnounceParser_getName();

/* Deconstructor: Deletes the instance of the command parser */
void CommandAnnounceParser_destroy(CommandAnnounceParserHandle self);

#endif
