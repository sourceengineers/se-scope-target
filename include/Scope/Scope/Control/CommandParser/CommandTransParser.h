/*!*****************************************************************************
 * @file         CommandTransParser.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the configurator for the ev_trans.
 *
 *              
 ******************************************************************************/

#ifndef COMMANDTRANSPARSER_H_
#define COMMANDTRANSPARSER_H_

#include <Scope/Control/Command/ICommand.h>
#include <Scope/Communication/Receiver.h>
#include <Scope/Control/Command/CommandTrans.h>
#include <Scope/GeneralPurpose/DataTypes.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommandTransParserPrivateData* CommandTransParserHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the command parser */
CommandTransParserHandle CommandTransParser_create(IScopeHandle scope);

/* Configurates the gives command */
ICommandHandle CommandTransParser_getCommand(CommandTransParserHandle self);

/* Returns the name of the command */
char* CommandTransParser_getName();

/* Deconstructor: Deletes the instance of the command parser */
void CommandTransParser_destroy(CommandTransParserHandle self);

#endif
