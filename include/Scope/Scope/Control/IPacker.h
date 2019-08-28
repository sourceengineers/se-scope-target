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

#include <Scope/GeneralPurpose/BufferedByteStream.h>
#include <Scope/GeneralPurpose/IIntStream.h>
#include <Scope/GeneralPurpose/DataTypes.h>
#include <Scope/GeneralPurpose/FloatRingBuffer.h>

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
    void (* pack)(IPackerHandle packer);

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
                            const uint32_t timestamp);

    /**
     * Prepares flow control
     * @param packer
     * @param flowcontrol
     */
    void (* prepareFlowControl)(IPackerHandle packer, const char* flowcontrol);

    /**
     * Prepares address announcement
     * @param packer
     * @param name
     * @param type
     * @param address
     */
    void (* prepareAddressAnnouncement)(IPackerHandle packer, const char* name, const char* type,
                                        const ADDRESS_DATA_TYPE address);

    void (* prepareDetect)(IPackerHandle packer);

    /**
     * Resets the packer
     * @param packer
     */
    void (* reset)(IPackerHandle packer);

} IPacker;

#endif
