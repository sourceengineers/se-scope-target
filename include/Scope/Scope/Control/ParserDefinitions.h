/*!*****************************************************************************
 * @file         ParserDefinitions.h
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
 *               is not valid. In these cases the purchased commercial license is applied.
 *               Please contact us at scope@sourceengineers.com for a commercial license.
 * }
 *
 * @authors      Samuel Schuepbach <samuel.schuepbach@sourceengineers.com>
 *
 * @brief        Defines keywords used by the packer and unpacker
 *
 ******************************************************************************/

#ifndef PARSERDEFINITIONS_H_
#define PARSERDEFINITIONS_H_

#include "Scope/Core/ScopeTypes.h"

#include <stddef.h>

extern const char* KEYWORD_PAYLOAD;
extern const char* KEYWORD_TRANSPORT;
extern const char* KEYWORD_SC_DATA;
extern const char* KEYWORD_PLACEHOLDER;
extern const char* KEYWORD_ANNOUNCE;
extern const char* KEYWORD_FLOW_CTRL;
extern const char* KEYWORD_CL_DATA;
extern const char* KEYWORD_T_STMP;
extern const char* KEYWORD_T_INC;
extern const char* KEYWORD_SC_DETECT;
extern const char* KEYWORD_TGR;
extern const char* KEYWORD_TGR_FOUND;
extern const char* KEYWORD_TGR_CL_DATA_IND;
extern const char* KEYWORD_TGR_CL_ID;
extern const char* KEYWORD_NUMBER_OF_CHANNELS;
extern const char* KEYWORD_SC_CMD;
extern const char* FLOWCONTROL_ACK;
extern const char* FLOWCONTROL_NAK;
extern const char* KEYWORD_TRUE;
extern const char* KEYWORD_FALSE;
extern const char* KEYWORD_CF_TGR_MODE_CONTINOUS;
extern const char* KEYWORD_CF_TGR_MODE_NORMAL;
extern const char* KEYWORD_CF_TGR_MODE_ONESHOT;
extern const char* KEYWORD_CF_TGR_EDGE_RISING;
extern const char* KEYWORD_CF_TGR_EDGE_FALLING;
extern const char* KEYWORD_CF_TGR_EDGE;
extern const char* KEYWORD_CF_TGR_CL_ID;
extern const char* KEYWORD_CF_TGR_MODE;
extern const char* KEYWORD_CF_TGR_LEVEL;
extern const char* KEYWORD_VERSION;
extern const char* KEYWORD_TIME_BASE;
extern const char* KEYWORD_CHANNELS;
extern const size_t KEYWORD_ANNOUNCE_LENGTH;
extern const size_t KEYWORD_FLOW_CTRL_LENGTH;
extern const size_t KEYWORD_CHANNELS_LENGTH;
extern const size_t FLOWCONTROL_ACK_LENGTH;
extern const size_t FLOWCONTROL_NAK_LENGTH;
extern const size_t KEYWORD_NUMBER_OF_CHANNELS_LENGTH;
extern const size_t KEYWORD_SC_DATA_LENGTH;
extern const size_t KEYWORD_TGR_LENGTH;
extern const size_t KEYWORD_TGR_MODE_LENGTH;
extern const size_t KEYWORD_SC_DETECT_LENGTH;
extern const size_t KEYWORD_TGR_FOUND_LENGTH;
extern const size_t KEYWORD_TGR_CL_DATA_IND_LENGTH;
extern const size_t KEYWORD_TGR_CL_ID_LENGTH;
extern const size_t KEYWORD_CL_DATA_LENGTH;
extern const size_t KEYWORD_PAYLOAD_LENGTH;
extern const size_t KEYWORD_TRUE_LENGTH;
extern const size_t KEYWORD_FALSE_LENGTH;
extern const size_t KEYWORD_T_STMP_LENGTH;
extern const size_t KEYWORD_T_INC_LENGTH;
extern const size_t KEYWORD_TGR_MODE_MAX_LENGTH;
extern const size_t KEYWORD_VERSION_LENGTH;
extern const size_t KEYWORD_TIME_BASE_LENGTH;
extern const size_t MAX_COMMAND_LENGTH; // Max length of a command
extern const size_t MAX_FIELD_LENGTH; // Max length of a field

#endif
