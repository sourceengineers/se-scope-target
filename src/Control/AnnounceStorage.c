/*!*********************************************************************************************************************
 * @file         AnnounceStorage.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 **********************************************************************************************************************/

#include "Scope/Control/AnnounceStorage.h"
#include "Scope/GeneralPurpose/DataTypes.h"

#include "Scope/Core/ScopeTypes.h"
#include "Scope/Version.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __AnnounceStoragePrivateData{
    size_t configuredAmountOfAddresses;
    AddressDefinition* addresses;
    size_t maxAmountOfAddresses;
    size_t maxAmountOfChannels;
    float timeBase;
    char* version;
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
    self->configuredAmountOfAddresses = 0;
    self->addresses = malloc(sizeof(AddressDefinition) * maxAmountOfAddresses);
    assert(self->addresses);
    self->maxAmountOfAddresses = maxAmountOfAddresses;
    self->version = SE_SCOPE_TARGET_VERSION;
    self->timeBase = timeBase;
    self->maxAmountOfChannels = maxAmountOfChannels;

    /* Initialize all addresses to have a defined state */
    for(size_t i = 0; i < maxAmountOfAddresses; ++i){
        self->addresses[i].type = SE_FLOAT;
        self->addresses[i].address = 0;
        self->addresses[i].name[0] = '\0';
    }

    return self;
}

size_t AnnounceStorage_getMaxAmountOfChannels(AnnounceStorageHandle self){
    return self->maxAmountOfChannels;
}

float AnnounceStorage_getTimeBase(AnnounceStorageHandle self){
    return self->timeBase;
}

char* AnnounceStorage_getVersion(AnnounceStorageHandle self){
    return self->version;
}

size_t AnnounceStorage_getMaxAmountOfAddresses(AnnounceStorageHandle self){
    return self->maxAmountOfAddresses;
}

AddressDefinition* AnnounceStorage_getAddressToTransmit(AnnounceStorageHandle self, const uint32_t addressId){
    return &self->addresses[addressId];
}

size_t AnnounceStorage_getAmountOfConfiguredAddresses(AnnounceStorageHandle self){
    return self -> configuredAmountOfAddresses;
}

bool AnnounceStorage_addAnnounceAddress(AnnounceStorageHandle self, const char* name, const void* address,
                                       const DATA_TYPES type){

    if(self->configuredAmountOfAddresses >= self->maxAmountOfAddresses){
        return false;
    }

    if(strlen(name) > MAX_LENGTH_OF_ANNOUNCE_NAME){
        return false;
    }

    self->addresses[self->configuredAmountOfAddresses].type = type;
    self->addresses[self->configuredAmountOfAddresses].address = (ADDRESS_DATA_TYPE) address;
    strncpy(self->addresses[self->configuredAmountOfAddresses].name, name, MAX_LENGTH_OF_ANNOUNCE_NAME-1);

    self->configuredAmountOfAddresses++;
    return true;
}

void AnnounceStorage_clearAddresses(AnnounceStorageHandle self){
    /* Set all addresses to have a defined state */
    for(size_t i = 0; i < self->maxAmountOfAddresses; ++i){
        self->addresses[i].type = SE_FLOAT;
        self->addresses[i].address = 0;
        self->addresses[i].name[0] = '\0';
    }
    self->configuredAmountOfAddresses = 0;
}

void AnnounceStorage_destroy(AnnounceStorageHandle self){
    free(self->addresses);
    self->addresses = NULL;
    free(self);
    self = NULL;
}
