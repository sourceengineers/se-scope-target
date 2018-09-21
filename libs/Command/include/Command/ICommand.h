/*!****************************************************************************************************************************************
 * @file         IFloatStream.h
 *
 * @copyright    Copyright (c) 2018 by Sonnen. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Streaming interface
 *               open: Opens the channel to a given float*
 *               getSize: Returns the size of data ready to be read 
 *               getStream: Writes the ready data into the float array
 *               close: Closes the stream 
 *
 *****************************************************************************************************************************************/

#ifndef ICOMMAND_H_
#define ICOMMAND_H_

typedef struct ICommandStruct* ICommandHandle;

typedef struct ICommandStruct {
  void* implementer;
  void (*run)(IFloatStreamHandle self);
  size_t(*setCommandAttribute)(IFloatStreamHandle self, void* attr);
} ICommand ;

#endif
