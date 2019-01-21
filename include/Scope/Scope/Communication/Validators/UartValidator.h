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
 
#ifndef UARTVALIDATOR_H_
#define UARTVALIDATOR_H_

#include <Scope/Communication/IComValidator.h>
#include <Scope/Communication/Validators/CommunicationCommons.h>
#include <Scope/Communication/IComValidator.h>
#include <Scope/GeneralPurpose/DataTypes.h>

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
