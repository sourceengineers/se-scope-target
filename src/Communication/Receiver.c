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
  IUnpacker unpacler;
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
ReceiverHandle Receiver_create(IUnpackerHandle unpacler, IByteStreamHandle byteStream, IComValidatorHandle validator,
                               SenderHandle sender){

  ReceiverHandle self = malloc(sizeof(ReceiverPrivateData));

  self->unpacler = *unpacler;
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
  bool parsingIsValid = self->unpacler.unpack(&self->unpacler, (const char*) data, length);

  /* If parsing failed, send a NAK */
  if(parsingIsValid == false){
    Sender_flowControl(self->sender, FLOWCONTROL_NAK);
    Sender_transmit(self->sender);
    return false;
  }

  /* Check if checks are valid */
  if(self->validator->checkPresentInProtocol(self->validator) == true){

    const size_t lengthOfCheck = self->unpacler.getLengthOfCheck(&self->unpacler);
    const size_t lengthOfBytesToCheck = self->unpacler.getLengthOfBytesToCheck(&self->unpacler);

    uint8_t check[lengthOfCheck];
    uint8_t bytesToCheck[lengthOfBytesToCheck];

    self->unpacler.getCheck(&self->unpacler, check);
    self->unpacler.getBytesToCheck(&self->unpacler, bytesToCheck);

    bool transportIsValid = self->validator->validateCheck(self->validator, (const uint8_t*) check, lengthOfCheck,
                                                           (const uint8_t*) bytesToCheck, lengthOfBytesToCheck);
    /* Send a NAK if the check failed */
    if(transportIsValid == false){
      Sender_flowControl(self->sender, FLOWCONTROL_NAK);
      Sender_transmit(self->sender);
      return false;
    }
  }

  Sender_flowControl(self->sender, FLOWCONTROL_ACK);
  Sender_transmit(self->sender);
  /* Accept the new data as new commands */
  self->unpacler.activateNewMessage(&self->unpacler);
  self->byteStream->flush(self->byteStream);
  return true;
}

IUnpackerHandle Receiver_getIUnpacker(ReceiverHandle self){
  return &self->unpacler;
}

void Receiver_destroy(ReceiverHandle self){
  free(self);
  self = NULL;
}
