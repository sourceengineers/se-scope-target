/*!*****************************************************************************
 * @file         CommandPackAnnounceParser.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the configurator for the ev_pack.
 *
 *              
 ******************************************************************************/

#ifndef COMMANDPACKANNOUNCEPARSER_H_
#define COMMANDPACKANNOUNCEPARSER_H_

#include <Scope/Control/Command/CommandPackAnnounce.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandPackAnnounceParserPrivateData* CommandPackAnnounceParserHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the command parser */
CommandPackAnnounceParserHandle CommandPackAnnounceParser_create(IScopeHandle scope, IPackerHandle packer);

/* Configurates the gives command */
ICommandHandle CommandPackAnnounceParser_getCommand(CommandPackAnnounceParserHandle self);

/* Returns the name of the command */
char* CommandPackAnnounceParser_getName(void);

/* Deconstructor: Deletes the instance of the command parser */
void CommandPackAnnounceParser_destroy(CommandPackAnnounceParserHandle self);

#endif
