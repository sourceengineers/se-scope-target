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
  IByteStreamHandle byteStream;
  IComValidatorHandle validator;

} RecieverPrivateData ;

/******************************************************************************
 Private functions
******************************************************************************/

/******************************************************************************
 Public functions
******************************************************************************/
RecieverHandle Reciever_create(IUnpackerHandle iUnpacker, IByteStreamHandle byteStream, IComValidatorHandle validator){

  RecieverHandle self = malloc(sizeof(RecieverPrivateData));

  self->iUnpacker = *iUnpacker;
  self->byteStream = byteStream;
  self->validator =  validator;

  return self;
}

bool Reciever_unpack(RecieverHandle self){

  /* Check if Parser is able to parse the given data */
  const size_t length = self->byteStream->length(self->byteStream);
  uint8_t data[length];
  self->byteStream->read(self->byteStream, data, length);
  bool parsingIsValid = self->iUnpacker.unpack(&self->iUnpacker, (const char*) data, length);
  if(parsingIsValid == false){
    return false;
  }

  /* Check if checks are valid */
  if(self->validator->checkPresentInProtocol(self->validator) == true){

    const size_t lengthOfCheck = self->iUnpacker.getLengthOfCheck(&self->iUnpacker);
    const size_t lengthOfBytesToCheck = self->iUnpacker.getLengthOfBytesToCheck(&self->iUnpacker);

    uint8_t check[lengthOfCheck];
    uint8_t bytesToCheck[lengthOfBytesToCheck];

    self->iUnpacker.getCheck(&self->iUnpacker, check);
    self->iUnpacker.getBytesToCheck(&self->iUnpacker, bytesToCheck);

    bool transportIsValid = self->validator->validateCheck(self->validator, (const uint8_t*) check, lengthOfCheck,
                                                           (const uint8_t*) bytesToCheck, lengthOfBytesToCheck);
    if(transportIsValid == false){
      return false;
    }
  }

  /* Accept the new data as new commands */
  self->iUnpacker.activateNewMessage(&self->iUnpacker);
  self->byteStream->flush(self->byteStream);
  return true;
}

IUnpackerHandle Reciever_getIUnpacker(RecieverHandle self){
  return &self->iUnpacker;
}

void Reciever_destroy(RecieverHandle self){
  free(self);
  self = NULL;
}
