/*!*****************************************************************************
 * @file         CommandRunning.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the configurator for the cf_running.
 *
 *              
 ******************************************************************************/
 
#ifndef COMMANDRUNNINGPARSER_H_
#define COMMANDRUNNINGPARSER_H_

#include <Command/ICommand.h>
#include <Communication/Unpacker.h>
#include <Command/CommandRunning.h>


/* Defines class */
typedef struct __CommandRunningParserPrivateData* CommandRunningParserHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instanze of the channel */
CommandRunningParserHandle CommandRunningParser_create(ICommandHandle iCommand, IUnpackerHandle iUnpacker);

/* Configurates the gives channel */
void CommandRunningParser_configure(CommandRunningParserHandle self);

/* Deconstructor: Deletes the instanze of the channel */
void CommandRunningParser_destroy(CommandRunningParserHandle self);

#endif
