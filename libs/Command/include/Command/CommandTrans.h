/*!*****************************************************************************
 * @file         CommandTrans.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the ev_trans command
 *                
 *               
 ******************************************************************************/
 
#ifndef COMMANDTRANS_H_
#define COMMANDTRANS_H_

#include <Scope/IScope.h>
#include <Command/ICommand.h>


/* Defines class */
typedef struct __CommandTransPrivateData* CommandTransHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instanze of the channel */
CommandTransHandle CommandTrans_create(IScopeHandle iScope);

/* Deconstructor: Deletes the instanze of the channel */
void CommandTrans_destroy(CommandTransHandle self);

/* Returns the command interface */
ICommandHandle CommandTrans_getICommand(CommandTransHandle self);

/* Returns the name of the command */
const char* CommandTrans_getName(CommandTransHandle self);


#endif
