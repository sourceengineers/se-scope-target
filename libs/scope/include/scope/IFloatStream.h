/*!****************************************************************************************************************************************
 * @file         IFloatStream.h
 *
 * @copyright    Copyright (c) 2018 by Sonnen. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samue.schuepbach@sourceengineers.com
 *
 * @brief        TODO
 *
 *****************************************************************************************************************************************/

#ifndef IFloatStream_H_
#define IFloatStream_H_

#include <unistd.h>

typedef struct IFloatStreamStruct* IFloatStreamHandle;

struct IFloatStreamStruct {
  void* implementer;
  void (*open)(IFloatStreamHandle self, float* floatStream);
  size_t(*getSize)(IFloatStreamHandle self);
  size_t(*getStream)(IFloatStreamHandle self);
  void (*close)(IFloatStreamHandle self);
};

#endif
