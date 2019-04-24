/*!****************************************************************************************************************************************
 * @file         AddressStorage.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Control/AddressStorage.h>
#include <Scope/Core/ScopeTypes.h>
#include <Scope/GeneralPurpose/DataTypes.h>

#include <stdlib.h>
#include <string.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __AddressStoragePrivateData{

    AddressDefinition* addresses;
    size_t maxAmountOfAddresses;
    IObserverHandle packObserver;

} AddressStoragePrivateData;

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
    for(int i = 0; i < maxAmountOfAddresses; ++i){
        self->addresses[i].type = SE_FLOAT;
        self->addresses[i].address = 0;
        self->addresses[i].name[0] = '\0';
    }

    return self;
}

void AddressStorage_attachObserver(AddressStorageHandle self, IObserverHandle observer){
    self->packObserver = observer;
}

void AddressStorage_announce(AddressStorageHandle self){
    PACK_TYPES packType = PACK_ANNOUNCE;
    self->packObserver->update(self->packObserver, &packType);
}

size_t AddressStorage_getMaxAmountOfAddresses(AddressStorageHandle self){
    return self->maxAmountOfAddresses;
}

AddressDefinition* AddressStorage_getAddressToTransmit(AddressStorageHandle self, const uint32_t addressId){
    return &self->addresses[addressId];
}

void AddressStorage_addAnnounceAddress(AddressStorageHandle self, const char* name, const void* address,
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

void AddressStorage_destroy(AddressStorageHandle self){
    free(self->addresses);
    self->addresses = NULL;
    free(self);
    self = NULL;
}