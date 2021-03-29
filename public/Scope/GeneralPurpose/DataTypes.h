/*!****************************************************************************************************************************************
 * @file         Types.h
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

/**
 * Enum representing the data types
 */
typedef enum  __DATA_TYPES {
    SE_INT8,
    SE_INT16,
    SE_INT32,
    SE_UINT8,
    SE_UINT16,
    SE_UINT32,
    SE_FLOAT
} DATA_TYPES ;

#ifndef VP_SIZE
    #define ADDRESS_DATA_TYPE uint32_t
#endif

/* Choose the right integer length */
#if (VP_SIZE==4)
#define ADDRESS_DATA_TYPE uint32_t
#elif(VP_SIZE==8)
#define ADDRESS_DATA_TYPE uint64_t
#endif

#define SeScopeGenericReference void*


#endif
