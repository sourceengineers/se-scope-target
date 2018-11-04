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

#include <Scope/Command/ICommand.h>
#include <Scope/Communication/Receiver.h>
#include <Scope/Command/CommandRunning.h>
#include <Scope/GeneralPurpose/DataTypes.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandRunningParserPrivateData* CommandRunningParserHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the command parser */
CommandRunningParserHandle CommandRunningParser_create(ICommandHandle iCommand, IUnpackerHandle iUnpacker);

/* Configurates the gives command */
void CommandRunningParser_configure(CommandRunningParserHandle self);

/* Deconstructor: Deletes the instance of the command parser */
void CommandRunningParser_destroy(CommandRunningParserHandle self);

#endif
