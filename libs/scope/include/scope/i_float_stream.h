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

#include <unistd.h>

typedef struct IFloatStreamStruct IFloatStream;

struct IFloatStreamStruct {
  
  size_t(*getSize)(IFloatStream* self);
  float(*getData)(IFloatStream* self);
};
