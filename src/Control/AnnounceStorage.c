/*!****************************************************************************************************************************************
 * @file         AnnounceStorage.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Control/AnnounceStorage.h>
#include <Scope/Core/ScopeTypes.h>
#include <Scope/GeneralPurpose/DataTypes.h>
#include <Version.h>

#include <stdlib.h>
#include <string.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __AnnounceStoragePrivateData{

    AddressDefinition* addresses;
    size_t maxAmountOfAddresses;
    size_t maxAmountOfChannels;
    float timeBase;
    const char* version;
    IObserverHandle packObserver;

} AnnounceStoragePrivateData;

/******************************************************************************
 Private functions
******************************************************************************/


/******************************************************************************
 Public functions
******************************************************************************/
AnnounceStorageHandle AnnounceStorage_create(const size_t maxAmountOfAddresses, const size_t maxAmountOfChannels, const float timeBase){

    AnnounceStorageHandle self = malloc(sizeof(AnnounceStoragePrivateData));
    assert(self);
    self->addresses = malloc(sizeof(AddressDefinition) * maxAmountOfAddresses);
    assert(self->addresses);
    self->maxAmountOfAddresses = maxAmountOfAddresses;
    self->version = SE_SCOPE_TARGET_VERSION;
    self->timeBase = timeBase;
    self->maxAmountOfChannels = maxAmountOfChannels;

    /* Initialize all addresses to have a defined state */
    for(int i = 0; i < maxAmountOfAddresses; ++i){
        self->addresses[i].type = SE_FLOAT;
        self->addresses[i].address = 0;
        self->addresses[i].name[0] = '\0';
    }

    return self;
}

void AnnounceStorage_attachObserver(AnnounceStorageHandle self, IObserverHandle observer){
    self->packObserver = observer;
}

size_t AnnounceStorage_getMaxAmountOfChannels(AnnounceStorageHandle self){
    return self->maxAmountOfChannels;
}

float AnnounceStorage_getTimeBase(AnnounceStorageHandle self){
    return self->timeBase;
}

void AnnounceStorage_getVersion(AnnounceStorageHandle self, char* version){
    strncpy(version, self->version, strlen(self->version));
}

void AnnounceStorage_announce(AnnounceStorageHandle self){
    PACK_TYPES packType = PACK_ANNOUNCE;
    self->packObserver->update(self->packObserver, &packType);
}

size_t AnnounceStorage_getMaxAmountOfAddresses(AnnounceStorageHandle self){
    return self->maxAmountOfAddresses;
}

AddressDefinition* AnnounceStorage_getAddressToTransmit(AnnounceStorageHandle self, const uint32_t addressId){
    return &self->addresses[addressId];
}

void AnnounceStorage_addAnnounceAddress(AnnounceStorageHandle self, const char* name, const void* address,
                                       const DATA_TYPES type,
                                       const uint32_t addressId){

    if(addressId >= self->maxAmountOfAddresses){
        return;
    }

    if(strlen(name) > maxAddrNameLength){
        return;
    }

    self->addresses[addressId].type = type;
    self->addresses[addressId].address = (ADDRESS_DATA_TYPE) address;
    strncpy(self->addresses[addressId].name, name, maxAddrNameLength);
}

void AnnounceStorage_destroy(AnnounceStorageHandle self){
    free(self->addresses);
    self->addresses = NULL;
    free(self);
    self = NULL;
}
