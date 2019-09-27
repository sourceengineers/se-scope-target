/*!*****************************************************************************
 * @file         CommandDetect.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the configurator for the cf_Detect.
 *
 *              
 ******************************************************************************/

#ifndef COMMANDDETECTPARSER_H_
#define COMMANDDETECTPARSER_H_

#include <Scope/Control/Commands/Command/CommandDetect.h>
#include <Scope/Control/IUnpacker.h>
#include <Scope/Control/ICommand.h>
#include <Scope/Core/IScope.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandDetectParserPrivateData* CommandDetectParserHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/**
 * Constructor
 * @param unpacker
 * @return
 */
CommandDetectParserHandle CommandDetectParser_create(IObserverHandle packOverser);

/**
 * Returns the command
 * @param self
 * @return
 */
ICommandHandle CommandDetectParser_getCommand(CommandDetectParserHandle self);

/**
 * Returns the name of the command
 * @return
 */
char* CommandDetectParser_getName(void);

/**
 * Deconstructor
 * @param self
 */
void CommandDetectParser_destroy(CommandDetectParserHandle self);

#endif
