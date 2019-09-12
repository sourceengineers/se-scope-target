/*!*****************************************************************************
 * @file         Keywords.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Defines keywords used by the packer and unpacker
 *
 ******************************************************************************/

#include <stddef.h>
#include <Scope/Core/ScopeTypes.h>

static const char* KEYWORD_PAYLOAD = "payload";
static const char* KEYWORD_TRANSPORT = "transport";
static const char* KEYWORD_SC_DATA = "sc_data";
static const char* KEYWORD_PLACEHOLDER = "...";
static const char* KEYWORD_ANNOUNCE = "sc_announce";
static const char* KEYWORD_FLOW_CTRL = "flow_ctrl";
static const char* KEYWORD_CL_DATA = "cl_data";
static const char* KEYWORD_T_STMP = "t_stmp";
static const char* KEYWORD_T_INC = "t_inc";
static const char* KEYWORD_SC_DETECT = "sc_detect";
static const char* KEYWORD_TGR = "tgr";
static const char* KEYWORD_TGR_FOUND = "found";
static const char* KEYWORD_TGR_CL_DATA_IND = "cl_data_ind";
static const char* KEYWORD_TGR_CL_ID = "cl_id";
static const char* KEYWORD_NUMBER_OF_CHANNELS = "cl_amount";
static const char* KEYWORD_SC_CMD = "sc_cmd";
static const char* FLOWCONTROL_ACK = "ACK";
static const char* FLOWCONTROL_NAK = "NAK";
static const char* KEYWORD_TRUE = "true";
static const char* KEYWORD_FALSE = "false";
static const char* KEYWORD_CF_TGR_MODE_CONTINOUS = "Continous";
static const char* KEYWORD_CF_TGR_MODE_NORMAL = "Normal";
static const char* KEYWORD_CF_TGR_MODE_ONESHOT = "OneShot";
static const char* KEYWORD_CF_TGR_EDGE_RISING = "rising";
static const char* KEYWORD_CF_TGR_EDGE_FALLING = "falling";
static const char* KEYWORD_CF_TGR_EDGE = "edge";
static const char* KEYWORD_CF_TGR_CL_ID = "cl_id";
static const char* KEYWORD_CF_TGR_MODE = "mode";
static const char* KEYWORD_CF_TGR_LEVEL = "level";
static const char* KEYWORD_VERSION = "version";
static const char* KEYWORD_TIME_BASE = "time_base";
static const char* KEYWORD_CHANNELS = "channels";
static const size_t KEYWORD_ANNOUNCE_LENGTH = 11;
static const size_t KEYWORD_FLOW_CTRL_LENGTH = 9;
static const size_t KEYWORD_CHANNELS_LENGTH = 8;
static const size_t FLOWCONTROL_ACK_LENGTH = 3;
static const size_t FLOWCONTROL_NAK_LENGTH = 3;
static const size_t KEYWORD_NUMBER_OF_CHANNELS_LENGTH = 9;
static const size_t KEYWORD_SC_DATA_LENGTH = 7;
static const size_t KEYWORD_TGR_LENGTH = 3;
static const size_t KEYWORD_TGR_MODE_LENGTH = 4;
static const size_t KEYWORD_SC_DETECT_LENGTH = 9;
static const size_t KEYWORD_TGR_FOUND_LENGTH = 5;
static const size_t KEYWORD_TGR_CL_DATA_IND_LENGTH = 11;
static const size_t KEYWORD_TGR_CL_ID_LENGTH = 5;
static const size_t KEYWORD_CL_DATA_LENGTH = 7;
static const size_t KEYWORD_PAYLOAD_LENGTH = 7;
static const size_t KEYWORD_TRUE_LENGTH = 4;
static const size_t KEYWORD_FALSE_LENGTH = 5;
static const size_t KEYWORD_T_STMP_LENGTH = 6;
static const size_t KEYWORD_T_INC_LENGTH = 5;
static const size_t KEYWORD_TGR_MODE_MAX_LENGTH = 30;
static const size_t KEYWORD_VERSION_LENGTH = 7;
static const size_t KEYWORD_TIME_BASE_LENGTH = 9;
static const size_t MAX_COMMAND_LENGTH = 30; // Max length of a command
static const size_t MAX_FIELD_LENGTH = 30; // Max length of a field

#define maxAddrNameLength 30 // Max length of an address name
/**
 * Definitions for the AnnounceStorage
 */
typedef struct{
    char name[maxAddrNameLength]; // Name of an address
    DATA_TYPES type; // Type of the address
    ADDRESS_DATA_TYPE address; // The address
} AddressDefinition;
