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
#include <Communication/CommunicationTypes.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __RecieverPrivateData* RecieverHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instanze of the reciever */
RecieverHandle Reciever_create(IUnpackerHandle iReciever, COM_TYPE comType);

/* Returns the IUnpacker interface */
IUnpackerHandle Reciever_getIUnpacker(RecieverHandle self);

/* Unpacks the data in the inputstream */
bool Reciever_unpack(RecieverHandle self, const char* data, const size_t length);

/* Deconstructor: Deletes the instanze of the reciever */
void Reciever_destroy(RecieverHandle self);


#endif
