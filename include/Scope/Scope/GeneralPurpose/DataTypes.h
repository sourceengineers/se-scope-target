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

/* Define the string names of the data types */
#define UINT8_NAME "UINT8"
#define UINT16_NAME "UINT16"
#define UINT32_NAME "UINT32"
#define UINT64_NAME "UINT64"
#define FLOAT_NAME "FLOAT"
#define DOUBLE_NAME "DOUBLE"

/* Define list of names for easy access, matching the DATA_TYPES enum entries */
const char* DATA_TYPE_NAMES[6] = {UINT8_NAME, UINT16_NAME, UINT32_NAME, UINT64_NAME, FLOAT_NAME, DOUBLE_NAME};

/* Define dataypes themselves */
typedef enum {UINT8, UINT16, UINT32, UINT64, FLOAT, DOUBLE} DATA_TYPES;

/* Define macro to fetch the right data type name */
#define getDataTypeName(type) DATA_TYPE_NAMES[type]

/* Choose the right integer length */
#if (ARCH_SIZE_32)
  #define gemmi_uint uint32_t
#else
  #define gemmi_uint uint64_t
#endif



#endif
