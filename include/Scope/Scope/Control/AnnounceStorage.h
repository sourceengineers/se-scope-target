/*!*****************************************************************************
 * @file         AnnounceStorage.h
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
typedef struct __AnnounceStoragePrivateData* AnnounceStorageHandle;



/******************************************************************************
 Public functions
******************************************************************************/
/**
 * Constructor
 * @param maxAmountOfAddresses the maximal amount of addresses which can be stored in the storage
 * @param timeBase Sets the time base with which will be polled. This depends on how the scope is implemented
 * @return
 */
AnnounceStorageHandle AnnounceStorage_create(const size_t maxAmountOfConfiguredAddresses,
        const size_t maxAmountOfChannels, const TIME_BASE timeBase);

/**
 * Sets a new definition of a address. If the addressId is bigger than the maximum amount of elements,
 * the function will return without doing anything
 * @param self
 * @param name name of the address
 * @param address address
 * @param type type of the address
 * @param addressId position into which the address should be written
 */
void AnnounceStorage_addAnnounceAddress(AnnounceStorageHandle self, const char* name, const void* address,
                                       const DATA_TYPES type,
                                       const uint32_t addressId);

/**
 * Returns the max amount of channels that can be configured in the system. This is used to let the host know
 * that no more than maxAmountOfChannels will be accepted.
 * @param self
 * @return
 */
size_t AnnounceStorage_getMaxAmountOfChannels(AnnounceStorageHandle self);

/**
 * Returns the timebase of the system
 * @param self
 * @return
 */
TIME_BASE AnnounceStorage_getTimeBase(AnnounceStorageHandle self);

/**
 * Attaches the packObserver to the AnnounceStorage. This can be used to transmit the addresses
 * @param self
 * @param observer packObserver
 */
void AnnounceStorage_attachObserver(AnnounceStorageHandle self, IObserverHandle observer);

/**
 * Get the version of the se scope
 * @param version
 */
void AnnounceStorage_getVersion(AnnounceStorageHandle self, char* version);

/**
 * Updates the packObserver, triggering a pending event in the packer
 * @param self
 */
void AnnounceStorage_announce(AnnounceStorageHandle self);

/**
 * Maximal amount of addresses that can be stored in the storage
 * @param self
 * @return
 */
size_t AnnounceStorage_getMaxAmountOfAddresses(AnnounceStorageHandle self);

/**
 * Returns the address stored in id.
 * @param self
 * @param addressId
 * @return Yiels NULL if id > AnnounceStorage_getMaxAmountOfAddresses
 */
AddressDefinition* AnnounceStorage_getAddressToTransmit(AnnounceStorageHandle self, const uint32_t addressId);

/**
 * Deconstructor
 * @param self
 */
void AnnounceStorage_destroy(AnnounceStorageHandle self);

#endif
