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

static const char* UINT8_NAME = "UINT8";
static const char* UINT16_NAME = "UINT16";
static const char* UINT32_NAME = "UINT32";
static const char* FLOAT_NAME = "FLOAT";

typedef enum {UINT8, UINT16, UINT32, UINT64, FLOAT, DOUBLE} DATA_TYPES;


#endif
