/*!*****************************************************************************
 * @file         CommandTInc.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the configurator for the cf_TInc.
 *
 *              
 ******************************************************************************/

#ifndef COMMANDTINCPARSER_H_
#define COMMANDTINCPARSER_H_

#include <Scope/Control/Command/ICommand.h>
#include <Scope/Control/Command/CommandTInc.h>
#include <Scope/GeneralPurpose/DataTypes.h>
#include <Scope/Control/IUnpacker.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandTIncParserPrivateData* CommandTIncParserHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the command parser */
CommandTIncParserHandle CommandTIncParser_create(IScopeHandle scope, IUnpackerHandle unpacker);

/* Configurates the gives command */
ICommandHandle CommandTIncParser_getCommand(CommandTIncParserHandle self);

/* Returns the name of the command */
char* CommandTIncParser_getName();

/* Deconstructor: Deletes the instance of the command parser */
void CommandTIncParser_destroy(CommandTIncParserHandle self);

#endif
