/*!*****************************************************************************
 * @file         CommandPackParser.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the configurator for the ev_pack.
 *
 *              
 ******************************************************************************/

#ifndef COMMANDPACKPARSER_H_
#define COMMANDPACKPARSER_H_

#include <Scope/Control/Command/ICommand.h>
#include <Scope/Control/Command/CommandPack.h>
#include <Scope/GeneralPurpose/DataTypes.h>
#include <Scope/Control/IPacker.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandPackParserPrivateData* CommandPackParserHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the command parser */
CommandPackParserHandle CommandPackParser_create(IScopeHandle scope, IPackerHandle packer);

/* Configurates the gives command */
ICommandHandle CommandPackParser_getCommand(CommandPackParserHandle self);

/* Returns the name of the command */
char* CommandPackParser_getName();

/* Deconstructor: Deletes the instance of the command parser */
void CommandPackParser_destroy(CommandPackParserHandle self);

#endif
