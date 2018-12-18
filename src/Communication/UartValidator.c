/*!****************************************************************************************************************************************
 * @file         Uart.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Communication/IComValidator.h>
#include <Scope/Communication/CommunicationCommons.h>
#include <Scope/Communication/UartValidator.h>

/******************************************************************************
 Define private data
******************************************************************************/
static const size_t CHECKSUM_BYTE_LENGTH = 4;


/* Class data */
typedef struct __UartValidatorPrivateData
{
  IComValidator validator;

} UartValidatorPrivateData ;

/* Generates a checksum over the presented data */
static void generate(uint8_t* checksum, const uint8_t* bytesToCheck, const size_t length);

/******************************************************************************
 Private functions
******************************************************************************/
static void generate(uint8_t* checksum, const uint8_t* bytesToCheck, const size_t length){

  gemmi_uint check = 0;

  for (int i = 0; i < length; ++i) {
    check += bytesToCheck[i];
  }

  for (int j = 0; j < 4; ++j) {
    checksum[j] = (uint8_t) ((check >> j * 8) & 0xFF);
  }
}

static bool validateCheck(IComValidatorHandle self, const uint8_t* check, const size_t lengthOfCheck,
                            const uint8_t* bytesToCheck, const size_t lengthOfBytesToCheck){

  if(lengthOfBytesToCheck > CHECKSUM_BYTE_LENGTH){
    return false;
  }

  uint8_t localCheck[CHECKSUM_BYTE_LENGTH];

  generate(localCheck, bytesToCheck, lengthOfBytesToCheck);

  for (int i = 0; i < CHECKSUM_BYTE_LENGTH; ++i) {
    if(localCheck[i] != check[i]){
      return false;
    }
  }
  return true;
}

static size_t getCheckLength(IComValidatorHandle self){
  return CHECKSUM_BYTE_LENGTH;
}

static void createCheck(IComValidatorHandle self, uint8_t* checksum, const uint8_t* bytesToCheck, const size_t length){

  generate(checksum, bytesToCheck, length);
}

static bool checkPresentInProtocol(IComValidatorHandle self){
  return true;
}
/******************************************************************************
 Public functions
******************************************************************************/
/* Constructor: Creates a new instance of the UartValidator */
UartValidatorHandle UartValidator_create(){

  UartValidatorHandle self = (UartValidatorHandle) malloc(sizeof(UartValidatorPrivateData));

  self->validator.implementer = self;
  self->validator.checkPresentInProtocol = &checkPresentInProtocol;
  self->validator.createCheck = &createCheck;
  self->validator.getCheckLength = &getCheckLength;
  self->validator.validateCheck = &validateCheck;

  return self;
}

IComValidatorHandle UartValidator_getIComValidator(UartValidatorHandle self){
  return &self->validator;
}

/* Deconstructor: Deletes the instance of the UartValidator */
void UartValidator_destroy(UartValidatorHandle self){
  free(self);
  self = NULL;
}
