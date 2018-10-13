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

#include <Command/ICommand.h>
#include <Communication/Reciever.h>
#include <Command/CommandTInc.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandTIncParserPrivateData* CommandTIncParserHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the command parser */
CommandTIncParserHandle CommandTIncParser_create(ICommandHandle iCommand, IUnpackerHandle iUnpacker);

/* Configurates the gives command */
void CommandTIncParser_configure(CommandTIncParserHandle self);

/* Deconstructor: Deletes the instance of the command parser */
void CommandTIncParser_destroy(CommandTIncParserHandle self);

#endif
