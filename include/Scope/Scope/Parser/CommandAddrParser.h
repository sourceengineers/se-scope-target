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

#include <Scope/Parser/Command/ICommand.h>
#include <Scope/Communication/Receiver.h>
#include <Scope/Parser/Command/CommandAddr.h>
#include <Scope/GeneralPurpose/DataTypes.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandAddrParserPrivateData* CommandAddrParserHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the command parser */
CommandAddrParserHandle CommandAddrParser_create(IScopeHandle scope, IUnpackerHandle unpacker);

/* Configurates the given command */
ICommandHandle CommandAddrParser_getCommand(CommandAddrParserHandle self);

/* Deconstructor: Deletes the instance of the parser */
void CommandAddrParser_destroy(CommandAddrParserHandle self);

/* Returns the name of the command */
char* CommandAddrParser_getName();

#endif
