/*!*****************************************************************************
 * @file         CommunicationFactory.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Factory to generate the needed
 *
 ******************************************************************************/ 
 
#ifndef COMMUNICATIONFACTORY_H_
#define COMMUNICATIONFACTORY_H_

#include <Communication/EthernetValidator.h>
#include <Communication/IComValidator.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __CommunicationFactoryPrivateData* CommunicationFactoryHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instance of the factory */
CommunicationFactoryHandle CommunicationFactory_create();
                                           
/* Deconstructor: Deletes the instance of the factory */
void CommunicationFactory_destroy(CommunicationFactoryHandle self);

/* Returns the command interface */
IComValidatorHandle CommunicationFactory_getIComValidator(CommunicationFactoryHandle self, COM_TYPE type);


#endif