/*!****************************************************************************************************************************************
 * @file         Reciever.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Communication/Reciever.h>

/* Define public data */
typedef struct __RecieverPrivateData
{
  IUnpacker iUnpacker;
  COM_TYPE comType;


} RecieverPrivateData ;

/* Public functions */
RecieverHandle Reciever_create(IUnpackerHandle iUnpacker, COM_TYPE comType){

  RecieverHandle self = malloc(sizeof(RecieverPrivateData));

  self->iUnpacker = *iUnpacker;

  return self;
}

IUnpackerHandle Reciever_getIUnpacker(RecieverHandle self){
  return &self->iUnpacker;
}

void Reciever_destroy(RecieverHandle self){
  free(self);
  self = NULL;
}
