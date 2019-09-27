/*!*****************************************************************************
 * @file         CommandPackDetectParser.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the configurator for the ev_pack.
 *
 *              
 ******************************************************************************/

#ifndef COMMANDPACKDETECTPARSER_H_
#define COMMANDPACKDETECTPARSER_H_

#include <Scope/Control/PackCommands/Command/CommandPackDetect.h>
#include <Scope/Control/ICommand.h>
#include <Scope/Control/IPacker.h>
#include <Scope/Core/IScope.h>
#include <Scope/Control/AnnounceStorage.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandPackDetectParserPrivateData* CommandPackDetectParserHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/**
 * Constructor
 * @param addressStorage
 * @param packer
 * @return
 */
CommandPackDetectParserHandle CommandPackDetectParser_create(IPackerHandle packer);

/**
 * Returns the command
 * @param self
 * @return
 */
ICommandHandle CommandPackDetectParser_getCommand(CommandPackDetectParserHandle self);

/**
 * Returns the name of the command
 * @return
 */
char* CommandPackDetectParser_getName(void);

/**
 * Deconstructor
 * @param self
 */
void CommandPackDetectParser_destroy(CommandPackDetectParserHandle self);

#endif
