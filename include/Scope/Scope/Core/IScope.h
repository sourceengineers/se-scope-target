/*!*****************************************************************************
 * @file         IScope.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Interface for functions which will be used by the commands.
 *               This interface will not be visible for parts outside of the
 *               scope
 *
 ******************************************************************************/

#ifndef ISCOPE_H_
#define ISCOPE_H_

#include <Scope/GeneralPurpose/IIntStream.h>
#include <Scope/GeneralPurpose/DataTypes.h>
#include <Scope/Core/ScopeTypes.h>
#include <Scope/GeneralPurpose/FloatRingBuffer.h>

/******************************************************************************
 Define interface handle data
******************************************************************************/
typedef struct IScopeStruct* IScopeHandle;

/******************************************************************************
 Define interface
******************************************************************************/
typedef struct IScopeStruct{
    GenericReference handle;

    /* Functions represention the various commands */
    void (* poll)(IScopeHandle scope);

    void (* setTimeIncrement)(IScopeHandle scope, uint32_t timeIncrement);

    uint32_t (* getTimeIncrement)(IScopeHandle scope);

    IIntStreamHandle (* getTimestamp)(IScopeHandle scope);

    void (* clear)(IScopeHandle scope);

    void (* configureTrigger)(IScopeHandle scope, TriggerConfiguration conf);

    void (* configureChannelAddress)(IScopeHandle scope, void* address,
                                     uint32_t idOfChangedChannel, DATA_TYPES typeOfAddress);

    void (* announce)(IScopeHandle scope);

    void (* transmit)(IScopeHandle scope);

    void (* setChannelRunning)(IScopeHandle scope, uint32_t idOfChangedChannel);

    void (* setChannelStopped)(IScopeHandle scope, uint32_t idOfChangedChannel);

    /* Helper functions */
    size_t (* getAmountOfChannels)(IScopeHandle scope);

    bool (* scopeIsReadyToSend)(IScopeHandle scope);

    bool (* dataIsReadyToSend)(IScopeHandle scope);

    bool (* announcementIsReadyToSend)(IScopeHandle scope);

    void (* dataIsTransmitted)(IScopeHandle scope);

    /* Data fetcher functions */
    TriggeredValues (* getTriggerData)(IScopeHandle scope);

    bool (* channelHasToBePacked)(IScopeHandle scope, uint32_t channelId);

    FloatRingBufferHandle (* getChannelBuffer)(IScopeHandle scope, uint32_t channelId);

    size_t (* getAmountOfUsedChannelData)(IScopeHandle scope, uint32_t channelId);

    AddressDefinition* (* getAnnounceAddressToTransmit)(IScopeHandle scope, uint32_t addressId);

    size_t (* getMaxAmmountOfAnnounceAddresses)(IScopeHandle scope);

    size_t (* getMaxSizeOfChannel)(IScopeHandle scope);

} IScope;

#endif
