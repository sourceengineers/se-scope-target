/*!****************************************************************************************************************************************
 * @file         JsonCommon.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach <samuel.schuepbach@sourceengineers.com>
 *
 * @brief        Contains specifications needed by both the JsonUnpacker as well as the JsonPacker
 *
 *****************************************************************************************************************************************/

#ifndef JSONCOMMON_H_
#define JSONCOMMON_H_

#include "Scope/GeneralPurpose/DataTypes.h"


#if (ARCH_SIZE_32)
#define MAX_LENGTH_OF_NUMBER 12
#else
#define MAX_LENGTH_OF_NUMBER 22
#endif

#define MAX_LENGTH_OF_FIELD_NAME 31

#endif
