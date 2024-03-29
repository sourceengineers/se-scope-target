/*!****************************************************************************************************************************************
 * @file         IPacker.h
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
 *               is not valid. In these cases the purchased commercial license is applied.
 *               Please contact us at scope@sourceengineers.com for a commercial license.
 * }
 *
 * @authors      Samuel Schuepbach <samuel.schuepbach@sourceengineers.com>
 *
 * @brief        Specifies an interface which must be used by a protocol to allows the scope to pack
 *               output data.
 *               This allows to extend the scope for multiple protocols.
 *
 *****************************************************************************************************************************************/

#ifndef IPACKER_H_
#define IPACKER_H_

#include "Scope/GeneralPurpose/DataTypes.h"

#include <se-lib-c/stream/BufferedByteStream.h>
#include <se-lib-c/stream/IIntStream.h>
#include <se-lib-c/container/FloatRingBuffer.h>
#include <se-lib-c/logger/ILogger.h>
#include "Scope/Version.h"
#include "Scope/Core/ScopeTypes.h"

typedef struct __ScDataChannelDef {
    FloatRingBufferHandle stream;
    uint32_t id;
} ScDataChannelDef;

typedef struct __ScAnnounceChannelDef {
    uint32_t id;
    char* name;
    DATA_TYPES type;
    uint32_t address;
} ScAnnounceChannelDef;

typedef struct __ScAnnounceMetaData {
    uint32_t maxChannels;
    float timebase;
    char* version;
} ScAnnounceMetaData;

typedef struct __ScDataTriggerDef {
    bool isTriggered;
    uint32_t channelId;
    uint32_t timestamp;
    TRIGGER_MODE triggerMode;
} ScDataTriggerDef;

typedef struct __ScLogDataDef{
	char message[50];
} ScLogDataDef;

/******************************************************************************
 Define interface handle data
******************************************************************************/
typedef struct IPackerStruct* IPackerHandle;

/******************************************************************************
 Define interface
******************************************************************************/
typedef struct IPackerStruct{
    SeScopeGenericReference handle;

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
    void (* addChannel)(IPackerHandle packer, ScDataChannelDef channel);

    /**
     * Prepares the time increment
     * @param packer
     * @param timeIncrement
     */
    void (* addTimeIncrement)(IPackerHandle packer, const uint32_t timeIncrement);

    /**
     * Prepares the timestamp
     * @param packer
     * @param timestamp Reference to the timestamp stream
     */
    void (* addTimestamp)(IPackerHandle packer, IIntStreamHandle timestamp);


    /**
        * Prepares the log
        * @param packer
        * @param timestamp Reference to the timestamp stream
        */
    void (* addLog)(IPackerHandle packer, ScLogDataDef log);

    /**
     * Prepares the trigger
     * @param packer
     * @param isTriggered
     * @param channelId
     * @param timestamp
     */
    void (* addTrigger)(IPackerHandle packer, ScDataTriggerDef trigger);

    /**
     * Prepares address announcement
     * @param packer
     * @param channel
     */
    void (* addAddressAnnouncement)(IPackerHandle packer, const ScAnnounceChannelDef address);

    /**
     * Prepares the packer to pack version, timebase and max channels
     * @param packer
     * @param meta
     */
    void (* addAnnouncement)(IPackerHandle packer, const ScAnnounceMetaData meta);

    /**
     * Resets the packer
     * @param packer
     */
    void (* reset)(IPackerHandle packer);

} IPacker;

#endif  //IPACKER_H_
