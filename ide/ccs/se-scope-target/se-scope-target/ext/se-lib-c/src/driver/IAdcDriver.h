/*!****************************************************************************************************************************************
 * @file         IAdcDriver.h
 *
 * @copyright    © by Source Engineers GmbH, 2019
 *
 * @authors      Flurin Bühler <flurin.buehler@sourceengineers.com>
 *
 * @brief        TODO
 * 
 *****************************************************************************************************************************************/

#ifndef IADCDRIVER_H_
#define IADCDRIVER_H_

#include <stdint.h>

// Declare the handle to the interface
typedef void* IAdcDriver_Handle;

// Declares the methods of the interface
typedef uint16_t (*IAdcDriver_getAdcValue)(IAdcDriver_Handle handle, uint16_t channelId);

// Declare the interface
typedef struct __IAdcDriver
{
    IAdcDriver_Handle handle;
    IAdcDriver_getAdcValue getAdcValue;
} IAdcDriver;

#endif //IADCDRIVER_H_
