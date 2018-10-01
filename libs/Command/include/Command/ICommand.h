/*!****************************************************************************************************************************************
 * @file         IFloatStream.h
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
 *
 *****************************************************************************************************************************************/

#ifndef ICOMMAND_H_
#define ICOMMAND_H_

#include <stdlib.h>

typedef struct ICommandStruct* ICommandHandle;

typedef struct ICommandStruct {
  void* implementer;
  void (*run)(ICommandHandle self);
  void(*setCommandAttribute)(ICommandHandle self, void* attr);
  const char*(*getCommandName)(ICommandHandle self);
} ICommand ;

#endif
