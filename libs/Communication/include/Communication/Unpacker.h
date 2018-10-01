/*!*****************************************************************************
 * @file         Unpacker.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the Unpacker.
 * 
 ******************************************************************************/
 
#ifndef UNPACKER_H_
#define UNPACKER_H_

#include <unistd.h>
#include <Communication/IUnpacker.h>

/* Defines class */
typedef struct __UnpackerPrivateData* UnpackerHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instanze of the channel */
UnpackerHandle Unpacker_create(IUnpackerHandle iUnpacker);

/* Deconstructor: Deletes the instanze of the channel */
void Unpacker_destroy(UnpackerHandle self);


#endif
