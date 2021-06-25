/*!*********************************************************************************************************************
 * @file         AnnounceStorage.c
 *
 * @copyright    Copyright (c) 2021 by Source Engineers GmbH. All Rights Reserved.
 *
 * @license {    This file is part of se-scope-target.
 *
 *               se-scope-target is free software; you can redistribute it and/or
 *               modify it under the terms of the GPLv3 General Public License Version 3
 *               as published by the Free Software Foundation.
 *
 *               se-scope-target is distributed in the hope that it will be useful,
 *               but WITHOUT ANY WARRANTY; without even the implied warranty of
 *               MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *               GNU General Public License for more details.
 *
 *               You should have received a copy of the GPLv3 General Public License Version 3
 *               along with se-scope-target.  If not, see <http://www.gnu.org/licenses/>.
 *
 *               In closed source or commercial projects, GPLv3 General Public License Version 3
 *               is not valid. In this case the commercial license received with the purchase
 *               is applied (See SeScopeLicense.pdf).
 *               Please contact us at scope@sourceengineers.com for a commercial license.
 * }
 *
 * @authors      Samuel Schuepbach <samuel.schuepbach@sourceengineers.com>
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
