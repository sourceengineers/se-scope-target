/*!*****************************************************************************
 * @file         EthernetValidator.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implements the IComValidator interface for the ethernet interface
 *
 *               Since the ethernet protocol already has checksums implemented,
 *               the Ehternet implementation is rather simple and will not have
 *               to do any checks
 * 
 ******************************************************************************/
 
#ifndef ETHERNETVALIDATOR_H_
#define ETHERNETVALIDATOR_H_

#include <Scope/Communication/IComValidator.h>
#include <Scope/Communication/CommunicationCommons.h>
#include <Scope/GeneralPurpose/DataTypes.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __EthernetValidatorPrivateData* EthernetValidatorHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the EthernetValidator */
EthernetValidatorHandle EthernetValidator_create();

IComValidatorHandle EthernetValidator_getIComValidator(EthernetValidatorHandle self);

/* Deconstructor: Deletes the instance of the EthernetValidator */
void EthernetValidator_destroy(EthernetValidatorHandle self);


#endif
