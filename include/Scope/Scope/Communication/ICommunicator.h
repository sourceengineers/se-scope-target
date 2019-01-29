/*!****************************************************************************************************************************************
 * @file         ICommunicator.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Interface, which will have to be implemented by every communication type to present functions
 *               to validate that the communication was successful.
 *
 *
 *
 *****************************************************************************************************************************************/

#ifndef ICOMMUNICATOR_H_
#define ICOMMUNICATOR_H_

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <Scope/GeneralPurpose/DataTypes.h>

/******************************************************************************
 Define interface handle data
******************************************************************************/
typedef struct ICommunicatorStruct* ICommunicatorHandle;

/******************************************************************************
 Define interface
******************************************************************************/
typedef struct ICommunicatorStruct {
  GenericReference handle;
  void (*runRx)(ICommunicatorHandle communicator);
  void (*runTx)(ICommunicatorHandle communicator);

} ICommunicator ;

#endif
