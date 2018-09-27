/*!****************************************************************************************************************************************
 * @file         IFloatStream.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
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

#ifndef IFloatStream_H_
#define IFloatStream_H_

#include <unistd.h>

typedef struct IFloatStreamStruct* IFloatStreamHandle;
typedef void* Shared_Safety_Util_Runnable_RunnableHandle;


typedef struct IFloatStreamStruct {
  void* implementer;
  void (*open)(IFloatStreamHandle self, float* floatStream);
  size_t(*getSize)(IFloatStreamHandle self);
  size_t(*getStream)(IFloatStreamHandle self);
  void (*close)(IFloatStreamHandle self);
} IFloatStream ;

#endif
