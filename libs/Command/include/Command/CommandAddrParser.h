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

#include <Command/ICommand.h>
#include <Communication/Unpacker.h>
#include <Command/CommandAddr.h>


/* Defines class */
typedef struct __CommandAddrParserPrivateData* CommandAddrParserHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instanze of the channel */
CommandAddrParserHandle CommandAddrParser_create(ICommandHandle iCommand, IUnpackerHandle iUnpacker);

/* Configurates the gives channel */
void CommandAddrParser_configure(CommandAddrParserHandle self);

/* Deconstructor: Deletes the instanze of the channel */
void CommandAddrParser_destroy(CommandAddrParserHandle self);

#endif
