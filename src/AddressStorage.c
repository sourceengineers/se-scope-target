/*!****************************************************************************************************************************************
 * @file         AddressStorage.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/AddressStorage.h>
#include <stdlib.h>
#include <string.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __AddressStoragePrivateData
{

  AddressDefinition* addresses;
  size_t maxAmountOfAddresses;

} AddressStoragePrivateData ;

/******************************************************************************
 Private functions
******************************************************************************/


/******************************************************************************
 Public functions
******************************************************************************/
AddressStorageHandle AddressStorage_create(const size_t maxAmountOfAddresses){

  AddressStorageHandle self = malloc(sizeof(AddressStoragePrivateData));

  self->addresses = malloc(sizeof(AddressDefinition) * maxAmountOfAddresses);
  self->maxAmountOfAddresses = maxAmountOfAddresses;

  /* Initialize all addresses to have a defined state */
  for (int i = 0; i < maxAmountOfAddresses; ++i) {
    self->addresses[i].type = FLOAT;
    self->addresses[i].address = 0;
    self->addresses[i].name[0] = '\0';
    self->addresses[i].hasToBeSent = false;
  }

  return self;
}

const size_t AddressStorage_getMaxAmountOfAddresses(AddressStorageHandle self){
  return self->maxAmountOfAddresses;
}

AddressDefinition* AddressStorage_getAddressToTransmit(AddressStorageHandle self, const gemmi_uint addressId){
  return &self->addresses[addressId];
}

void AddressStorage_flagAddressesAsSend(AddressStorageHandle self){

  for (int i = 0; i < self->maxAmountOfAddresses; ++i) {
    self->addresses[i].hasToBeSent = false;
  }
}

void AddressStorage_setAnnounceAddress(AddressStorageHandle self, const char* name, const void* address,
                                    const DATA_TYPES type,
                                    const gemmi_uint addressId){

  if(addressId >= self->maxAmountOfAddresses){
    return;
  }

  if(strnlen(name, maxAddrNameLength + 1) > maxAddrNameLength){
    return;
  }

  self->addresses[addressId].type = type;
  self->addresses[addressId].address = (const gemmi_uint) address;
  strncpy(self->addresses[addressId].name, name, maxAddrNameLength);
  self->addresses[addressId].hasToBeSent = true;
}

void AddressStorage_destroy(AddressStorageHandle self){
  free(self->addresses);
  self->addresses = NULL;
  free(self);
  self = NULL;
}