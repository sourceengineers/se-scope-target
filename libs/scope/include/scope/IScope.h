/*!*****************************************************************************
 * @file         Scope.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the Scope.
 * 
 ******************************************************************************/
 
#ifndef ISCOPE_H_
#define ISCOPE_H_

typedef struct IScopeStruct* IScopeHandle;

typedef struct IScopeStruct {
  void* implementer;
  void (*poll)(IScopeHandle self);
  void (*trans)(IScopeHandle self);
  void (*setTimeIncrement)(IScopeHandle self, int timeIncrement);
} IScope ;

#endif
