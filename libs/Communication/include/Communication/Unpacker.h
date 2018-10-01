/*!*****************************************************************************
 * @file         Unpacker.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the Unpacker.
 *
 *               Custom functions like "Unpacker_unpack()" will have to be defined in a next step. These will have to
 *               include options to check the communication specific fields, like checksums and so on.
 *
 *               The unpacker will have to be renamed to reciever.
 * 
 ******************************************************************************/
 
#ifndef UNPACKER_H_
#define UNPACKER_H_

#include <unistd.h>
#include <Communication/IUnpacker.h>

/* Defines possible communication types */

typedef enum {ETHERNET, RS232} COM_TYPE;


/* Defines class */
typedef struct __UnpackerPrivateData* UnpackerHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instanze of the channel */
UnpackerHandle Unpacker_create(IUnpackerHandle iUnpacker, COM_TYPE comType);

IUnpackerHandle Unpacker_getIUnpacker(UnpackerHandle self);

/* Deconstructor: Deletes the instanze of the channel */
void Unpacker_destroy(UnpackerHandle self);


#endif
