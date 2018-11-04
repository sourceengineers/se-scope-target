/*!*****************************************************************************
 * @file         Receiver.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the Receiver.
 *
 *               Custom functions like "Receiver_unpack()" will have to be defined in a next step. These will have to
 *               include options to check the communication specific fields, like checksums and so on.
 *
 *               The receiver will have to be renamed to receiver.
 * 
 ******************************************************************************/
 
#ifndef RECIEVER_H_
#define RECIEVER_H_

#include <unistd.h>
#include <Scope/Communication/IUnpacker.h>
#include <Scope/Communication/CommunicationCommons.h>
#include <Scope/GeneralPurpose/IByteStream.h>
#include <Scope/Communication/IComValidator.h>
#include <Scope/Communication/Sender.h>

const size_t MAX_FIELD_LENGTH = 30;
const size_t MAX_COMMAND_LENGTH = 30;

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __ReceiverPrivateData* ReceiverHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the receiver */
ReceiverHandle Receiver_create(IUnpackerHandle iUnpacker, IByteStreamHandle byteStream, IComValidatorHandle validator,
                               SenderHandle sender);

/* Returns the IUnpacker interface */
IUnpackerHandle Receiver_getIUnpacker(ReceiverHandle self);

/* Unpacks the data in the inputstream */
bool Receiver_unpack(ReceiverHandle self);

/* Deconstructor: Deletes the instance of the receiver */
void Receiver_destroy(ReceiverHandle self);


#endif
