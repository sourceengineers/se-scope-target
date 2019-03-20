/*!*****************************************************************************
 * @file         AddressStorage.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implements a storage module through which addresses can be named
 *               and transmitted.
 *               This allows the usage of the scope, even for non static addresses.
 *
 ******************************************************************************/

#ifndef ADDRESSSTORAGE_H_
#define ADDRESSSTORAGE_H_

#include <Scope/Control/ParserDefinitions.h>
#include <Scope/GeneralPurpose/DataTypes.h>
#include <Scope/GeneralPurpose/IObserver.h>

#include <stdint.h>
#include <stddef.h>
#include <stdint.h>

/******************************************************************************
 Define class handle data
******************************************************************************/
typedef struct __AddressStoragePrivateData* AddressStorageHandle;



/******************************************************************************
 Public functions
******************************************************************************/
/**
 * Constructor
 * @param maxAmountOfAddresses the maximal amount of addresses which can be stored in the storage
 * @return
 */
AddressStorageHandle AddressStorage_create(const size_t maxAmountOfAddresses);

/**
 * Sets a new definition of a address. If the addressId is bigger than the maximum amount of elements,
 * the function will return without doing anything
 * @param self
 * @param name name of the address
 * @param address address
 * @param type type of the address
 * @param addressId position into which the address should be written
 */
void AddressStorage_addAnnounceAddress(AddressStorageHandle self, const char* name, const void* address,
                                       const DATA_TYPES type,
                                       const uint32_t addressId);

/**
 * Attaches the packObserver to the AddressStorage. This can be used to transmit the addresses
 * @param self
 * @param observer packObserver
 */
void AddressStorage_attachObserver(AddressStorageHandle self, IObserverHandle observer);

/**
 * Updates the packObserver, triggering a pending event in the packer
 * @param self
 */
void AddressStorage_announce(AddressStorageHandle self);

/**
 * Maximal amount of addresses that can be stored in the storage
 * @param self
 * @return
 */
size_t AddressStorage_getMaxAmountOfAddresses(AddressStorageHandle self);

/**
 * Returns the address stored in id.
 * @param self
 * @param addressId
 * @return Yiels NULL if id > AddressStorage_getMaxAmountOfAddresses
 */
AddressDefinition* AddressStorage_getAddressToTransmit(AddressStorageHandle self, const uint32_t addressId);

/**
 * Deconstructor
 * @param self
 */
void AddressStorage_destroy(AddressStorageHandle self);

#endif
