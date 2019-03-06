/*!*****************************************************************************
 * @file         CommandPackDataParser.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the configurator for the ev_pack.
 *
 *              
 ******************************************************************************/

#ifndef COMMANDPACKDATAPARSER_H_
#define COMMANDPACKDATAPARSER_H_

#include <Scope/Control/Command/CommandPackData.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandPackDataParserPrivateData* CommandPackDataParserHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the command parser */
CommandPackDataParserHandle CommandPackDataParser_create(IScopeHandle scope, IPackerHandle packer);

/* Configurates the gives command */
ICommandHandle CommandPackDataParser_getCommand(CommandPackDataParserHandle self);

/* Returns the name of the command */
char* CommandPackDataParser_getName(void);

/* Deconstructor: Deletes the instance of the command parser */
void CommandPackDataParser_destroy(CommandPackDataParserHandle self);

#endif
