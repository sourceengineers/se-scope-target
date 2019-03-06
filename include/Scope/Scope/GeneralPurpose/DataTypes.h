/*!****************************************************************************************************************************************
 * @file         Types.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Defines different types in a enum to achieve a "template" kind
 *               of programming.
 *
 *****************************************************************************************************************************************/

#ifndef DATATYPES_H_
#define DATATYPES_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Define the string names of the data types */
#define SE_UINT8_NAME "SE_UINT8"
#define SE_UINT16_NAME "SE_UINT16"
#define SE_UINT32_NAME "SE_UINT32"
#define SE_UINT64_NAME "SE_UINT64"
#define SE_FLOAT_NAME "SE_FLOAT"
#define SE_DOUBLE_NAME "SE_DOUBLE"

/* Define list of names for easy access, matching the DATA_TYPES enum entries */
static const char* DATA_TYPE_NAMES[6] = {SE_UINT8_NAME, SE_UINT16_NAME, SE_UINT32_NAME, SE_UINT64_NAME, SE_FLOAT_NAME, SE_DOUBLE_NAME};

/* Define dataypes themselves */
typedef enum{
    SE_UINT8, SE_UINT16, SE_UINT32, SE_UINT64, SE_FLOAT, SE_DOUBLE
} DATA_TYPES;

/* Define macro to fetch the right data type name */
#define getDataTypeName(type) DATA_TYPE_NAMES[type]

/* Choose the right integer length */
#if (ARCH_SIZE_32)
#define ADDRESS_DATA_TYPE uint32_t
#else
#define ADDRESS_DATA_TYPE uint64_t
#endif

#define GenericReference void*


#endif
