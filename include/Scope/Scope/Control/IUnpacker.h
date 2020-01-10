/*!****************************************************************************************************************************************
 * @file         IUnpacker.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Specifies an interface which must be used by a protocol to allows the scope to unpack
 *               input data.
 *               This allows to extend the scope for multiple protocols.
 *
 *****************************************************************************************************************************************/

#ifndef IUNPACKER_H_
#define IUNPACKER_H_

#include "Scope/GeneralPurpose/DataTypes.h"
#include "Scope/Core/ScopeTypes.h"

/******************************************************************************
 Define interface handle datar
******************************************************************************/
typedef struct IUnpackerStruct* IUnpackerHandle;

typedef struct __CfRunningDef{
    uint32_t id;
    bool newState;
} CfRunningDef;

typedef struct __CfTriggerDef{
    float level;
    bool edge;
    TRIGGER_MODE mode;
    uint32_t channelId;
} CfTriggerDef;

typedef struct __CfAddressDef{
    uint32_t id;
    uint32_t address;
    DATA_TYPES type;
} CfAddressDef;

/******************************************************************************
 Define interface
******************************************************************************/
typedef struct IUnpackerStruct{
    SeScopeGenericReference handle;

    /**
     * Unpacks the data in the input stream
     * @param unpacker
     * @return
     */
    bool (* unpack)(IUnpackerHandle unpacker, MessageType type);

    /**
     * Returns the value contained in the ev_poll command
     * @param unpacker
     * @return
     */
    uint32_t (* evPoll_getTimestamp)(IUnpackerHandle unpacker);

    /**
     * Returns the amount of channels that will be sarted/stopped
     */
    size_t (* cfRunning_getAmount)(IUnpackerHandle unpacker);

    /**
     * Returns the new configuration for each channel
     * @param unpacker
     * @param index
     * @return
     */
    CfRunningDef (* cfRunning_getChannel)(IUnpackerHandle unpacker, uint32_t index);

    /**
     * Returns the trigger configuration
     * @param unpacker
     * @return
     */
    CfTriggerDef (* cfTrigger_getTriggerConfig)(IUnpackerHandle unpacker);

    /**
     * Returns the new t_inc value
     * @param unpacker
     * @return
     */
    uint32_t (* cfTInc_getInc)(IUnpackerHandle unpacker);

    /**
     * Returns the amount of addresses to be reconfigured
     * @param unpacker
     * @return
     */
    uint32_t (* cfAddress_getAmount)(IUnpackerHandle unpacker);

    /**
     * REturns the new channel configuration
     * @param unpacker
     * @param index
     * @return
     */
    CfAddressDef (* cfAddress_getChannel)(IUnpackerHandle unpacker, uint32_t index);

} IUnpacker;

#endif
