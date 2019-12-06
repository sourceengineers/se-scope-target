/*!****************************************************************************************************************************************
 * @file         IPacker.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Specifies an interface which must be used by a protocol to allows the scope to pack
 *               output data.
 *               This allows to extend the scope for multiple protocols.
 *
 *****************************************************************************************************************************************/

#ifndef IPACKER_H_
#define IPACKER_H_

#include "Scope/GeneralPurpose/DataTypes.h"

#include "Scope/GeneralPurpose/BufferedByteStream.h"
#include "Scope/GeneralPurpose/IIntStream.h"
#include "Scope/GeneralPurpose/FloatRingBuffer.h"
#include "Scope/Core/ScopeTypes.h"

/******************************************************************************
 Define interface handle data
******************************************************************************/
typedef struct IPackerStruct* IPackerHandle;

/******************************************************************************
 Define interface
******************************************************************************/
typedef struct IPackerStruct{
    GenericReference handle;

    /**
     * Calls the pack function of the packer. It packs the predefined data into the output stream
     * @param packer
     */
    void (* pack)(IPackerHandle packer, MessageType type);

    /**
     * Returns true if the packer is ready to serialize new data. This is the case if all previously sent data
     * has been sent and the transmit buffer is empty.
     * @param packer
     * @return
     */
    bool (* isReady)(IPackerHandle packer);

    /**
     * Prepares channels
     * @param packer
     * @param buffer Passes the Swap buffer into the packer
     * @param channelId id of the channel
     */
    void (* prepareChannel)(IPackerHandle packer, FloatRingBufferHandle buffer, const uint32_t channelId);

    /**
     * Prepares the time increment
     * @param packer
     * @param timeIncrement
     */
    void (* prepareTimeIncrement)(IPackerHandle packer, const uint32_t timeIncrement);

    /**
     * Prepares the timestamp
     * @param packer
     * @param timestamp Reference to the timestamp stream
     */
    void (* prepareTimestamp)(IPackerHandle packer, IIntStreamHandle timestamp);

    /**
     * Prepares the trigger
     * @param packer
     * @param isTriggered
     * @param channelId
     * @param timestamp
     */
    void (* prepareTrigger)(IPackerHandle packer, const bool isTriggered, const uint32_t channelId,
                            const uint32_t timestamp, char* triggerMode);

    /**
     * Prepares address announcement
     * @param packer
     * @param name
     * @param type
     * @param address
     */
    void (* prepareAddressAnnouncement)(IPackerHandle packer, const char* name, const char* type,
                                        const ADDRESS_DATA_TYPE address);

    /**
     * Prepares the packer to pack version, timebase and max channels
     * @param packer
     * @param timeBase
     * @param version
     * @param maxChannels
     */
    void (* prepareAnnouncement)(IPackerHandle packer, float timeBase, const char* version, size_t maxChannels);

    /**
     * Resets the packer
     * @param packer
     */
    void (* reset)(IPackerHandle packer);

} IPacker;

#endif
