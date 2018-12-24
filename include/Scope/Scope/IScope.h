/*!*****************************************************************************
 * @file         IScope.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Interface for functions which will be used by the commands.
 *               This interface will not be visible for parts outside of the
 *               scope
 *
 ******************************************************************************/

#ifndef ISCOPE_H_
#define ISCOPE_H_

#include <stdint.h>
#include <Scope/GeneralPurpose/IIntStream.h>
#include <Scope/GeneralPurpose/DataTypes.h>

/******************************************************************************
 Define interface handle data
******************************************************************************/
typedef struct IScopeStruct* IScopeHandle;

/******************************************************************************
 Define interface
******************************************************************************/
typedef struct IScopeStruct {
  void* implementer;
  void (*poll)(IScopeHandle scope, gemmi_uint timeStamp);
  void (*transmit)(IScopeHandle scope);
  void (*announce)(IScopeHandle scope);
  void (*setTimeIncrement)(IScopeHandle scope, gemmi_uint timeIncrement);
  bool (*transmitTimestampInc)(IScopeHandle scope);
  gemmi_uint (*getTimeIncrement)(IScopeHandle scope);
  IIntStreamHandle (*getTimestamp)(IScopeHandle scope);
  void (*clear)(IScopeHandle scope);
} IScope ;

#endif
