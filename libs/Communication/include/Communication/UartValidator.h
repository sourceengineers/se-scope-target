/*!*****************************************************************************
 * @file         UartValidator.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implements the IComValidator interface for the Uart interface
 * 
 ******************************************************************************/
 
#ifndef UartValidator_H_
#define UartValidator_H_

#include <Communication/IComValidator.h>
#include <Communication/CommunicationCommons.h>
#include "IComValidator.h"

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __UartValidatorPrivateData* UartValidatorHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the UartValidator */
UartValidatorHandle UartValidator_create();

IComValidatorHandle UartValidator_getIComValidator(UartValidatorHandle self);

/* Deconstructor: Deletes the instance of the UartValidator */
void UartValidator_destroy(UartValidatorHandle self);


#endif
