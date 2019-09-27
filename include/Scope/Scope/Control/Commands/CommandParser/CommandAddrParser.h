/*!*****************************************************************************
 * @file         CommandAddr.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the configurator for the cf_addr.
 *
 *              
 ******************************************************************************/

#ifndef COMMANDADDRPARSER_H_
#define COMMANDADDRPARSER_H_

#include <Scope/Control/Commands/Command/CommandAddr.h>
#include <Scope/Control/IUnpacker.h>
#include <Scope/Control/ICommand.h>
#include <Scope/Core/IScope.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandAddrParserPrivateData* CommandAddrParserHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/**
 * Constructor
 * @param scope
 * @param unpacker
 * @return
 */
CommandAddrParserHandle CommandAddrParser_create(IScopeHandle scope, IUnpackerHandle unpacker);

/**
 * Returns the command
 * @param self
 * @return
 */
ICommandHandle CommandAddrParser_getCommand(CommandAddrParserHandle self);

/**
 * Returns the name of the command
 * @return
 */
char* CommandAddrParser_getName(void);

/**
 * Deconstructor
 * @param self
 */
void CommandAddrParser_destroy(CommandAddrParserHandle self);

#endif
