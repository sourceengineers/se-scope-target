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

/******************************************************************************
 Define interface handle data
******************************************************************************/
typedef struct IScopeStruct* IScopeHandle;

/******************************************************************************
 Define interface
******************************************************************************/
typedef struct IScopeStruct {
  void* implementer;
  void (*poll)(IScopeHandle self);
  void (*trans)(IScopeHandle self);
  void (*setTimeIncrement)(IScopeHandle self, int timeIncrement);
} IScope ;

#endif
