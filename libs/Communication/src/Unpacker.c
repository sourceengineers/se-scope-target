/*!****************************************************************************************************************************************
 * @file         Unpacker.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Communication/Unpacker.h>

/* Define public data */
typedef struct __UnpackerPrivateData
{
  IUnpacker iUnpacker;
  COM_TYPE comType;


} UnpackerPrivateData ;

/* Public functions */
UnpackerHandle Unpacker_create(IUnpackerHandle iUnpacker, COM_TYPE comType){

  UnpackerHandle self = malloc(sizeof(UnpackerPrivateData));

  self->iUnpacker = *iUnpacker;

  return self;
}

IUnpackerHandle Unpacker_getIUnpacker(UnpackerHandle self){
  return &self->iUnpacker;
}

void Unpacker_destroy(UnpackerHandle self){
  free(self);
  self = NULL;
}
