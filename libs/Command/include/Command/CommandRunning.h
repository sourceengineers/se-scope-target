/*!*****************************************************************************
 * @file         Scope.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the Scope.
 * 
 ******************************************************************************/
 
#ifndef COMMANDRUNNING_H_
#define COMMANDRUNNING_H_

#include <Communication/Parser.h>

/* Defines class */
typedef struct __CommandRunningPrivateData* CommandRunningHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instanze of the channel */
CommandRunningHandle CommandRunning_create(Parser parser);

/* Deconstructor: Deletes the instanze of the channel */
void CommandRunning_destroy(CommandRunningHandle self);

#endif
