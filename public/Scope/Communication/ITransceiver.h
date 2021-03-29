/*!*****************************************************************************
 * @file         ITransceiver.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @license      This file is part of SE-Scope.
 *
 *               SE-Scope is free software; you can redistribute it and/or
 *               modify it under the terms of the GNU General Public License as
 *               published by the Free Software Foundation.
 *
 *               SE-Scope is distributed in the hope that it will be useful,
 *               but WITHOUT ANY WARRANTY; without even the implied warranty of
 *               MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *               GNU General Public License for more details.
 *
 *               You should have received a copy of the GNU General Public License
 *               along with SE-Scope.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Supplies an Interface to access the communication interfaces
 *
 ******************************************************************************/

#ifndef ITRANSCEIVER_H_
#define ITRANSCEIVER_H_

#include "Scope/GeneralPurpose/DataTypes.h"

/******************************************************************************
 Define interface handle data
******************************************************************************/
typedef struct ITransceiverStruct* ITransceiverHandle;

/**
 * Prototype of function that will be used as a callback
 */
typedef void(*TransmitCallback)(ITransceiverHandle self);

/******************************************************************************
 Define interface
******************************************************************************/
typedef struct ITransceiverStruct{
    SeScopeGenericReference handle;

    /**
     * Read data from the output stream
     * @param self
     * @param data
     * @param length
     * @return
     */
    void (* get)(ITransceiverHandle self,  uint8_t* data, size_t length);

    /**
     * Put data into the input stream so it can be interpreted
     * @param self
     * @param data
     * @param length
     */
    void (* put)(ITransceiverHandle self, uint8_t* data, size_t length);

    /**
     * Reset the input handler
     * @param self
     */
    void (* resetInput)(ITransceiverHandle self);

    /**
     * Reset the output handler
     * @param self
     */
    void (* resetOutput)(ITransceiverHandle self);

    /**
     * Bytes pending in the output buffer
     * @param self
     * @return
     */
    size_t (* outputSize)(ITransceiverHandle self);

} ITransceiver;

#endif
