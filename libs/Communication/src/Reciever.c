/*!****************************************************************************************************************************************
 * @file         Reciever.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Communication/Reciever.h>
#include <GeneralPurpose/IByteStream.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __RecieverPrivateData
{
  IUnpacker iUnpacker;
  COM_TYPE comType;

  /* The Reciever will later be implemented with a bytestream as a input method */
  IByteStream stream;
  uint8_t data[200];

} RecieverPrivateData ;

/* Prototype for strategy */
static bool (*checkTransportValidity)(RecieverHandle self);

/* Implementation of the Ethernet strategy */
static bool checkTransportEthernet(RecieverHandle self);

/******************************************************************************
 Private functions
******************************************************************************/
static bool checkTransportEthernet(RecieverHandle self){
  return true;
}

/******************************************************************************
 Public functions
******************************************************************************/
RecieverHandle Reciever_create(IUnpackerHandle iUnpacker, COM_TYPE comType){

  RecieverHandle self = malloc(sizeof(RecieverPrivateData));

  self->iUnpacker = *iUnpacker;
  self->comType = comType;

  switch(comType){
    case ETHERNET:
      checkTransportValidity = &checkTransportEthernet;
      break;

    default:
      checkTransportValidity = &checkTransportEthernet;
  }

  return self;
}

bool Reciever_unpack(RecieverHandle self, const char* data, const size_t length){

  /* Check if Parser is able to parse the given data */
  bool parsingIsValid = self->iUnpacker.unpack(&self->iUnpacker, data, length);
  if(parsingIsValid == false){
    return false;
  }

  /* Check if checksum and such are valid */
  bool transportIsValid = checkTransportValidity(self);
  if(transportIsValid == false){
    return false;
  }

  /* Accept the new data as new commands */
  self->iUnpacker.activateNewMessage(&self->iUnpacker);
  return true;
}

IUnpackerHandle Reciever_getIUnpacker(RecieverHandle self){
  return &self->iUnpacker;
}

void Reciever_destroy(RecieverHandle self){
  free(self);
  self = NULL;
}
