/*!*****************************************************************************
 * @file         CommandTInc.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the ev_t_inc command
 *                
 *               
 ******************************************************************************/
 
#ifndef COMMANDTINC_H_
#define COMMANDTINC_H_

#include <Scope/IScope.h>
#include <Command/ICommand.h>


/* Defines class */
typedef struct __CommandTIncPrivateData* CommandTIncHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instanze of the channel */
CommandTIncHandle CommandTInc_create(IScopeHandle iScope);

/* Deconstructor: Deletes the instanze of the channel */
void CommandTInc_destroy(CommandTIncHandle self);

/* Returns the command interface */
ICommandHandle CommandTInc_getICommand(CommandTIncHandle self);

/* Returns the name of the command */
const char* CommandTInc_getName(CommandTIncHandle self);


#endif
