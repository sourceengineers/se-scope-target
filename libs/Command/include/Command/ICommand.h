/*!****************************************************************************************************************************************
 * @file         IFloatStream.h
 *
 * @copyright    Copyright (c) 2018 by Sonnen. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Streaming interface
 *               run: Runs the command
 *               setCommandAttribute: Sets the attributes needed by the command
 *               to run.
 *
 *****************************************************************************************************************************************/

#ifndef ICOMMAND_H_
#define ICOMMAND_H_

typedef struct ICommandStruct* ICommandHandle;

typedef struct ICommandStruct {
  void* implementer;
  void (*run)(ICommandHandle self);
  void(*setCommandAttribute)(ICommandHandle self, void* attr);
} ICommand ;

#endif
