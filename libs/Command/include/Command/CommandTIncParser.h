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


/* Defines class */
typedef struct __CommandTIncParserPrivateData* CommandTIncParserHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instanze of the channel */
CommandTIncParserHandle CommandTIncParser_create(ICommandHandle iCommand, IUnpackerHandle iUnpacker);

/* Configurates the gives channel */
void CommandTIncParser_configure(CommandTIncParserHandle self);

/* Deconstructor: Deletes the instanze of the channel */
void CommandTIncParser_destroy(CommandTIncParserHandle self);

#endif
