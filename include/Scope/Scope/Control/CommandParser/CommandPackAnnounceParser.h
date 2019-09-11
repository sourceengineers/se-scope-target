/*!*****************************************************************************
 * @file         CommandPackAnnounceParser.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the configurator for the ev_pack.
 *
 *              
 ******************************************************************************/

#ifndef COMMANDPACKANNOUNCEPARSER_H_
#define COMMANDPACKANNOUNCEPARSER_H_

#include <Scope/Control/Command/CommandPackAnnounce.h>
#include <Scope/Control/Command/ICommand.h>
#include <Scope/Control/IPacker.h>
#include <Scope/Core/IScope.h>
#include <Scope/Control/AnnounceStorage.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandPackAnnounceParserPrivateData* CommandPackAnnounceParserHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/**
 * Constructor
 * @param addressStorage
 * @param packer
 * @return
 */
CommandPackAnnounceParserHandle CommandPackAnnounceParser_create(AnnounceStorageHandle addressStorage, IPackerHandle packer);

/**
 * Returns the command
 * @param self
 * @return
 */
ICommandHandle CommandPackAnnounceParser_getCommand(CommandPackAnnounceParserHandle self);

/**
 * Returns the name of the command
 * @return
 */
char* CommandPackAnnounceParser_getName(void);

/**
 * Deconstructor
 * @param self
 */
void CommandPackAnnounceParser_destroy(CommandPackAnnounceParserHandle self);

#endif
