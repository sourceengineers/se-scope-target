/*!*****************************************************************************
 * @file         AddressStorage.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Impelments the channels, which will be attached to the scope ???
 *
 ******************************************************************************/

#ifndef ADDRESSSTORAGE_H_
#define ADDRESSSTORAGE_H_

#include <stddef.h>
#include <Scope/GeneralPurpose/DataTypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <Scope/Core/ScopeTypes.h>
#include <Scope/GeneralPurpose/DataTypes.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __AddressStoragePrivateData* AddressStorageHandle;



/******************************************************************************
 Public functions
******************************************************************************/
/* Constructor: Creates a new instance of the channel */
AddressStorageHandle AddressStorage_create(const size_t maxAmountOfAddresses);

/* Sets a new definition of a address. If the addressId is bigger than the maximum amount of elements,
 * the function will return without doing anything */
void AddressStorage_addAnnounceAddress(AddressStorageHandle self, const char* name, const void* address,
                                    const DATA_TYPES type,
                                    const uint32_t addressId);

/* Returns how many addresses can be configured at maximum */
size_t AddressStorage_getMaxAmountOfAddresses(AddressStorageHandle self);

/* Returns a pointer to a address */
AddressDefinition* AddressStorage_getAddressToTransmit(AddressStorageHandle self, const uint32_t addressId);

/* Flags the storage ready to transmit */
void AddressStorage_announce(AddressStorageHandle self);

/* Checks if the addresses have to be sent */
bool AddressStorage_hasToBeSent(AddressStorageHandle self);

/* Flags the storage as sent */
void AddressStorage_hasBeenSent(AddressStorageHandle self);

/* Flags all Addresses as send */
void AddressStorage_flagAddressesAsSend(AddressStorageHandle self);

/* Deconstructor: Deletes the instance of the channel */
void AddressStorage_destroy(AddressStorageHandle self);

#endif
