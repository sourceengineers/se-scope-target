/*!****************************************************************************************************************************************
 * @file         Ethernet.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Communication/IComValidator.h>
#include <Communication/ComTypes.h>
#include <Communication/EthernetValidator.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __EthernetValidatorPrivateData
{
  IComValidator validator;

} EthernetValidatorPrivateData ;

/******************************************************************************
 Private functions
******************************************************************************/
static const bool validateCheck(IComValidatorHandle self, const uint8_t* check, const size_t lengthOfCheck,
                            const uint8_t* bytesToCheck, const size_t lengthOfBytesToCheck){
  return true;
}

static const size_t getCheckLength(IComValidatorHandle self){
  return 0;
}

static void createCheck(IComValidatorHandle self, uint8_t* checksum, const uint8_t* bytesToCheck, const size_t length){
  return;
}

static const bool checkPresentInProtocol(IComValidatorHandle self){
  return false;
}
/******************************************************************************
 Public functions
******************************************************************************/
/* Constructor: Creates a new instance of the EthernetValidator */
EthernetValidatorHandle EthernetValidator_create(){

  EthernetValidatorHandle self = (EthernetValidatorHandle) malloc(sizeof(EthernetValidatorPrivateData));

  self->validator.implementer = self;
  self->validator.checkPresentInProtocol = &checkPresentInProtocol;
  self->validator.createCheck = &createCheck;
  self->validator.getCheckLength = &getCheckLength;
  self->validator.validateCheck = &validateCheck;

  return self;
}

IComValidatorHandle EthernetValidator_getIComValidator(EthernetValidatorHandle self){
  return &self->validator;
}

/* Deconstructor: Deletes the instance of the EthernetValidator */
void EthernetValidator_destroy(EthernetValidatorHandle self){
  free(self);
  self = NULL;
}