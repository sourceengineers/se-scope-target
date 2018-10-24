/*!****************************************************************************************************************************************
 * @file         Receiver.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Communication/Receiver.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __ReceiverPrivateData
{
  IUnpacker iUnpacker;
  IByteStreamHandle byteStream;
  IComValidatorHandle validator;
  SenderHandle sender;

} ReceiverPrivateData ;

/******************************************************************************
 Private functions
******************************************************************************/

/******************************************************************************
 Public functions
******************************************************************************/
ReceiverHandle Receiver_create(IUnpackerHandle iUnpacker, IByteStreamHandle byteStream, IComValidatorHandle validator,
                               SenderHandle sender){

  ReceiverHandle self = malloc(sizeof(ReceiverPrivateData));

  self->iUnpacker = *iUnpacker;
  self->byteStream = byteStream;
  self->validator =  validator;
  self->sender = sender;

  return self;
}

bool Receiver_unpack(ReceiverHandle self){

  /* Check if Parser is able to parse the given data */
  const size_t length = self->byteStream->length(self->byteStream);
  uint8_t data[length];
  self->byteStream->read(self->byteStream, data, length);
  bool parsingIsValid = self->iUnpacker.unpack(&self->iUnpacker, (const char*) data, length);

  if(parsingIsValid == false){
    Sender_flowControl(self->sender, FLOWCONTROL_NAK);
    Sender_transmit(self->sender);
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
      Sender_flowControl(self->sender, FLOWCONTROL_NAK);
      Sender_transmit(self->sender);
      return false;
    }
  }

  Sender_flowControl(self->sender, FLOWCONTROL_ACK);
  Sender_transmit(self->sender);
  /* Accept the new data as new commands */
  self->iUnpacker.activateNewMessage(&self->iUnpacker);
  self->byteStream->flush(self->byteStream);
  return true;
}

IUnpackerHandle Receiver_getIUnpacker(ReceiverHandle self){
  return &self->iUnpacker;
}

void Receiver_destroy(ReceiverHandle self){
  free(self);
  self = NULL;
}
