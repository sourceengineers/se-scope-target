/*!*****************************************************************************
 * @file         IComValidator.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 ******************************************************************************/

#ifndef ICOMVALIDATOR_H
#define ICOMVALIDATOR_H

/******************************************************************************
 Define interface handle data
******************************************************************************/
typedef struct IComValidatorStruct* IComValidatorHandle;

/******************************************************************************
 Define interface
******************************************************************************/
typedef struct IComValidatorStruct {
  GenericReference handle;
  bool (*rxDataReady)(IComValidatorHandle validator);
  void (*txReadyToValidate)(IComValidatorHandle validator);

} IComValidator ;
#endif
