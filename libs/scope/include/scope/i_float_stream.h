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

typedef struct IFloatStreamStruct {

  size_t(*getSize)(struct IFloatStreamStruct *self);
  float(*getData)(struct IFloatStreamStruct *self);

} IFloatStream ;
