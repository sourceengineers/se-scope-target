/*!****************************************************************************************************************************************
 * @file         CommunicationTypes.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Defines different communication types. New types should only be added, once their implementation
 *               is ready to be used.
 *
 *****************************************************************************************************************************************/
 
#ifndef COMMUNICATIONTYPES_H_
#define COMMUNICATIONTYPES_H_

static const char* FLOWCONTROL_ACK = "ACK";
static const char* FLOWCONTROL_NAK = "NAK";

static const size_t MAX_FIELD_LENGTH = 30;
static const size_t MAX_COMMAND_LENGTH = 30;

/* Defines possible communication types */
typedef enum {ETHERNET, UART} COM_TYPE;

#endif
