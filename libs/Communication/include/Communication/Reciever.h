/*!*****************************************************************************
 * @file         Reciever.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the Reciever.
 *
 *               Custom functions like "Reciever_unpack()" will have to be defined in a next step. These will have to
 *               include options to check the communication specific fields, like checksums and so on.
 *
 *               The reciever will have to be renamed to reciever.
 * 
 ******************************************************************************/
 
#ifndef RECIEVER_H_
#define RECIEVER_H_

#include <unistd.h>
#include <Communication/IUnpacker.h>
#include <Communication/CommunicationCommons.h>
#include <GeneralPurpose/IByteStream.h>
#include <Communication/IComValidator.h>
#include <Communication/Sender.h>

static const size_t MAX_FIELD_LENGTH = 30;
static const size_t MAX_COMMAND_LENGTH = 30;

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __RecieverPrivateData* RecieverHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the reciever */
RecieverHandle Reciever_create(IUnpackerHandle iUnpacker, IByteStreamHandle byteStream, IComValidatorHandle validator,
                               SenderHandle sender);

/* Returns the IUnpacker interface */
IUnpackerHandle Reciever_getIUnpacker(RecieverHandle self);

/* Unpacks the data in the inputstream */
bool Reciever_unpack(RecieverHandle self);

/* Deconstructor: Deletes the instance of the reciever */
void Reciever_destroy(RecieverHandle self);


#endif
