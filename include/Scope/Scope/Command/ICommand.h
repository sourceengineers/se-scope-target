/*!****************************************************************************************************************************************
 * @file         ICommand.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Streaming interface
 *               run: Runs the command
 *               setCommandAttribute: Sets the attributes needed by the command
 *               to run. The data passed to the void pointer, is specified by 
 *               every command on its own.
 *               getCommandName: returns the name of the command
 *
 *****************************************************************************************************************************************/

#ifndef ICOMMAND_H_
#define ICOMMAND_H_

#include <stdlib.h>
#include <Scope/GeneralPurpose/DataTypes.h>

/******************************************************************************
 Define interface handle data
******************************************************************************/
typedef struct ICommandStruct* ICommandHandle;

/******************************************************************************
 Define interface
******************************************************************************/
typedef struct ICommandStruct {
  void* implementer;
  void (*run)(ICommandHandle self);
  void(*setCommandAttribute)(ICommandHandle self, void* attr);
  char*(*getCommandName)(ICommandHandle self);
} ICommand ;

#endif
