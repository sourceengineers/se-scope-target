/*!****************************************************************************************************************************************
 * @file         ICheckable.h
 *
 * @copyright    © by Source Engineers GmbH, 2019
 *
 * @authors      Flurin Bühler <flurin.buehler@sourceengineers.com>
 *
 * @brief        TODO
 * 
 *****************************************************************************************************************************************/

#ifndef ICHECKABLE_H_
#define ICHECKABLE_H_

#include <se-lib-c/definition/SeLibCTypes.h>
#include <stdbool.h>

// Declare the handle to the interface
typedef struct __ICheckable* ICheckableHandle;

// Declares the methods of the interface
typedef void (*ICheckable_setActive)(ICheckableHandle handle, bool isActive);
typedef bool (*ICheckable_isCheckOk)(ICheckableHandle handle);

// Declare the interface
typedef struct __ICheckable
{
    SeLibGenericReferece handle;
    ICheckable_setActive setActive;
    ICheckable_isCheckOk isCheckOk;
} ICheckable;

#endif //ICHECKABLE_H_
