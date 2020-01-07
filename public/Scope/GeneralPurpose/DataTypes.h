/*!****************************************************************************************************************************************
 * @file         Types.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @license This file is part of "se-scope-target". It is release under a commercial 
 *          license. Refer to the license PDF file received together with this code 
 *          for the exact licensing terms. 
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Defines the data types that can be used by the scope.
 *               It specifies the size of addresses by defines which can be controlled by setting
 *               the ARCH_SIZE_32. ARCH_SIZE_32=1 equals a 32 bit architecture and
 *               ARCH_SIZE_32=0, or not setting ARCH_SIZE_32 at all, defined a 64 bit architecture.
 *               This is necessary to be able to read from the correct address, and that the channels can be configured
 *               properly
 *
 *****************************************************************************************************************************************/

#ifndef DATATYPES_H_
#define DATATYPES_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Define the string names of the data types */
#define SE_INT8_NAME "SE_INT8"
#define SE_INT16_NAME "SE_INT16"
#define SE_INT32_NAME "SE_INT32"
#define SE_UINT8_NAME "SE_UINT8"
#define SE_UINT16_NAME "SE_UINT16"
#define SE_UINT32_NAME "SE_UINT32"
#define SE_FLOAT_NAME "SE_FLOAT"

/** Define list of names for easy access, matching the DATA_TYPES enum entries */
static const char* DATA_TYPE_NAMES[7] = {SE_INT8_NAME, SE_INT16_NAME, SE_INT32_NAME, SE_UINT8_NAME, SE_UINT16_NAME, SE_UINT32_NAME, SE_FLOAT_NAME};

/**
 * Enum representing the data types
 */
typedef enum{
    SE_INT8,
    SE_INT16,
    SE_INT32,
    SE_UINT8,
    SE_UINT16,
    SE_UINT32,
    SE_FLOAT,
} DATA_TYPES;

/** Define macro to fetch the right data type name */
#define getDataTypeName(type) DATA_TYPE_NAMES[type]

/* Choose the right integer length */
#if (ARCH_SIZE_32)
#define ADDRESS_DATA_TYPE uint32_t
#else
#define ADDRESS_DATA_TYPE uint64_t
#endif

#define GenericReference void*


#endif
