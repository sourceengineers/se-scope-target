/*!*****************************************************************************
 * @file         ParserDefinitions.c
 *
 * @copyright    Copyright (c) 2021 by Source Engineers GmbH. All Rights Reserved.
 *
 * @license {    This file is part of se-scope-target.
 *
 *               se-scope-target is free software; you can redistribute it and/or
 *               modify it under the terms of the GPLv3 General Public License Version 3
 *               as published by the Free Software Foundation.
 *
 *               se-scope-target is distributed in the hope that it will be useful,
 *               but WITHOUT ANY WARRANTY; without even the implied warranty of
 *               MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *               GNU General Public License for more details.
 *
 *               You should have received a copy of the GPLv3 General Public License Version 3
 *               along with se-scope-target.  If not, see <http://www.gnu.org/licenses/>.
 *
 *               In closed source or commercial projects, GPLv3 General Public License Version 3
 *               is not valid. In this case the commercial license received with the purchase
 *               is applied (See SeScopeLicense.pdf).
 *               Please contact us at scope@sourceengineers.com for a commercial license.
 * }
 *
 * @authors      Samuel Schuepbach <samuel.schuepbach@sourceengineers.com>
 *
 * @brief        Defines keywords used by the packer and unpacker
 *
 ******************************************************************************/

#include "Scope/Control/ParserDefinitions.h"

const char* KEYWORD_PAYLOAD = "payload";
const char* KEYWORD_TRANSPORT = "transport";
const char* KEYWORD_SC_DATA = "sc_data";
const char* KEYWORD_PLACEHOLDER = "...";
const char* KEYWORD_ANNOUNCE = "sc_announce";
const char* KEYWORD_FLOW_CTRL = "flow_ctrl";
const char* KEYWORD_CL_DATA = "cl_data";
const char* KEYWORD_T_STMP = "t_stmp";
const char* KEYWORD_T_INC = "t_inc";
const char* KEYWORD_SC_DETECT = "sc_detect";
const char* KEYWORD_TGR = "tgr";
const char* KEYWORD_TGR_FOUND = "found";
const char* KEYWORD_TGR_CL_DATA_IND = "cl_data_ind";
const char* KEYWORD_TGR_CL_ID = "cl_id";
const char* KEYWORD_NUMBER_OF_CHANNELS = "cl_amount";
const char* KEYWORD_SC_CMD = "sc_cmd";
const char* FLOWCONTROL_ACK = "ACK";
const char* FLOWCONTROL_NAK = "NAK";
const char* KEYWORD_TRUE = "true";
const char* KEYWORD_FALSE = "false";
const char* KEYWORD_CF_TGR_MODE_CONTINOUS = "Continous";
const char* KEYWORD_CF_TGR_MODE_NORMAL = "Normal";
const char* KEYWORD_CF_TGR_MODE_ONESHOT = "OneShot";
const char* KEYWORD_CF_TGR_EDGE_RISING = "rising";
const char* KEYWORD_CF_TGR_EDGE_FALLING = "falling";
const char* KEYWORD_CF_TGR_EDGE = "edge";
const char* KEYWORD_CF_TGR_CL_ID = "cl_id";
const char* KEYWORD_CF_TGR_MODE = "mode";
const char* KEYWORD_CF_TGR_LEVEL = "level";
const char* KEYWORD_VERSION = "version";
const char* KEYWORD_TIME_BASE = "time_base";
const char* KEYWORD_CHANNELS = "channels";
const size_t KEYWORD_ANNOUNCE_LENGTH = 11;
const size_t KEYWORD_FLOW_CTRL_LENGTH = 9;
const size_t KEYWORD_CHANNELS_LENGTH = 8;
const size_t FLOWCONTROL_ACK_LENGTH = 3;
const size_t FLOWCONTROL_NAK_LENGTH = 3;
const size_t KEYWORD_NUMBER_OF_CHANNELS_LENGTH = 9;
const size_t KEYWORD_SC_DATA_LENGTH = 7;
const size_t KEYWORD_TGR_LENGTH = 3;
const size_t KEYWORD_TGR_MODE_LENGTH = 4;
const size_t KEYWORD_SC_DETECT_LENGTH = 9;
const size_t KEYWORD_TGR_FOUND_LENGTH = 5;
const size_t KEYWORD_TGR_CL_DATA_IND_LENGTH = 11;
const size_t KEYWORD_TGR_CL_ID_LENGTH = 5;
const size_t KEYWORD_CL_DATA_LENGTH = 7;
const size_t KEYWORD_PAYLOAD_LENGTH = 7;
const size_t KEYWORD_TRUE_LENGTH = 4;
const size_t KEYWORD_FALSE_LENGTH = 5;
const size_t KEYWORD_T_STMP_LENGTH = 6;
const size_t KEYWORD_T_INC_LENGTH = 5;
const size_t KEYWORD_TGR_MODE_MAX_LENGTH = 30;
const size_t KEYWORD_VERSION_LENGTH = 7;
const size_t KEYWORD_TIME_BASE_LENGTH = 9;
const size_t MAX_COMMAND_LENGTH = 30; // Max length of a command
const size_t MAX_FIELD_LENGTH = 30; // Max length of a field
