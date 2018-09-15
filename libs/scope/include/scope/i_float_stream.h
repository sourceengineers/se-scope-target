/*!****************************************************************************************************************************************
 * @file         I_Float_Stream.h
 *
 * @copyright    Copyright (c) 2018 by Sonnen. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samue.schuepbach@sourceengineers.com
 *
 * @brief        TODO
 *
 *****************************************************************************************************************************************/

#ifndef I_FLOAT_STREAM_H_
#define I_FLOAT_STREAM_H_

#include <unistd.h>

typedef struct IFloatStreamStruct IFloatStream;

struct IFloatStreamStruct {
  void* implementer;
  void (*open)(IFloatStream* self, float* floatStream);
  size_t(*getSize)(IFloatStream* self);
  size_t(*getStream)(IFloatStream* self);
  void (*close)(IFloatStream* self);
};

#endif
