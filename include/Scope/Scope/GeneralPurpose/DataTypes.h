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

#define UINT8_NAME "UINT8"
#define UINT16_NAME "UINT16"
#define UINT32_NAME "UINT32"
#define UINT64_NAME "UINT64"
#define FLOAT_NAME "FLOAT"
#define DOUBLE_NAME "DOUBLE"

static char* const DATA_TYPE_NAMES[6] = {UINT8_NAME, UINT16_NAME, UINT32_NAME, UINT64_NAME, FLOAT_NAME, DOUBLE_NAME};

typedef enum {UINT8, UINT16, UINT32, UINT64, FLOAT, DOUBLE} DATA_TYPES;


#define getDataTypeName(type) DATA_TYPE_NAMES[type]


#endif
