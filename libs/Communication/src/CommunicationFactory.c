/*!****************************************************************************************************************************************
 * @file         CommunicationFactory.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Communication/CommunicationFactory.h>
#include <string.h>
#include <Communication/EthernetValidator.h>
#include <UartValidator.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommunicationFactoryPrivateData
{

  EthernetValidatorHandle ethernet;
  UartValidatorHandle uart;

} CommunicationFactoryPrivateData ;


/******************************************************************************
 Public functions
******************************************************************************/
CommunicationFactoryHandle CommunicationFactory_create(){

  CommunicationFactoryHandle self = malloc(sizeof(CommunicationFactoryPrivateData));

  self->ethernet = NULL;
  self->uart = NULL;

  return self;
}

IComValidatorHandle CommunicationFactory_getIComValidator(CommunicationFactoryHandle self, COM_TYPE type){

  if(type == ETHERNET){
    if(self->ethernet == NULL){
      self->ethernet = EthernetValidator_create();
    }
    return EthernetValidator_getIComValidator(self->ethernet);
  } else if(type == UART){
      if(self->uart == NULL){
        self->uart = UartValidator_create();
      }
    return UartValidator_getIComValidator(self->uart);
  }
  return NULL;
}


void CommunicationFactory_destroy(CommunicationFactoryHandle self){

  if(self->ethernet != NULL){
    EthernetValidator_destroy(self->ethernet);
  }
  if(self->uart != NULL){
    UartValidator_destroy(self->uart);
  }

  free(self);
  self = NULL;
}
