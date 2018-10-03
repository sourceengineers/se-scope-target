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
 *               The Reciever will have to be renamed to reciever.
 * 
 ******************************************************************************/
 
#ifndef RECIEVER_H_
#define RECIEVER_H_

#include <unistd.h>
#include <Communication/IUnpacker.h>

/* Defines possible communication types */

typedef enum {ETHERNET, RS232} COM_TYPE;


/* Defines class */
typedef struct __RecieverPrivateData* RecieverHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instanze of the channel */
RecieverHandle Reciever_create(IUnpackerHandle iReciever, COM_TYPE comType);

IUnpackerHandle Reciever_getIUnpacker(RecieverHandle self);

/* Deconstructor: Deletes the instanze of the channel */
void Reciever_destroy(RecieverHandle self);


#endif
