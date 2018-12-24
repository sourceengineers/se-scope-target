/*!****************************************************************************************************************************************
 * @file         IComValidator.h
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

#ifndef ICOMVALIDATOR_H_
#define ICOMVALIDATOR_H_

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <Scope/GeneralPurpose/DataTypes.h>

/******************************************************************************
 Define interface handle data
******************************************************************************/
typedef struct IComValidatorStruct* IComValidatorHandle;

/******************************************************************************
 Define interface
******************************************************************************/
typedef struct IComValidatorStruct {
  void* implementer;

	/**
	 * @param check The check which gets from the received package.
	 * @param bytesToCheck Bytes which have to be used to create the check.
	 */
  bool (*validateCheck)(IComValidatorHandle self, const uint8_t* check, const size_t lengthOfCheck,
                              const uint8_t* bytesToCheck, const size_t lengthOfBytesToCheck);
  size_t (*getCheckLength)(IComValidatorHandle self);

	/**
	 *
	 * @param checksum The array in which the created check gets written into. The size of this array can not be
	 * 									larger than what was returned from getCheckLength.
	 */
	void (*createCheck)(IComValidatorHandle self, uint8_t* checksum, const uint8_t* bytesToCheck, const size_t length);
  bool (*checkPresentInProtocol)(IComValidatorHandle self);

} IComValidator ;

#endif
