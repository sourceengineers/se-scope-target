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
#include <Scope/Core/ScopeTypes.h>

/******************************************************************************
 Define interface handle data
******************************************************************************/
typedef struct IScopeStruct* IScopeHandle;

/******************************************************************************
 Define interface
******************************************************************************/
typedef struct IScopeStruct {
  	GenericReference handle;
  	void (*poll)(IScopeHandle scope);
  	void (*setTimeIncrement)(IScopeHandle scope, uint32_t timeIncrement);
  	uint32_t (*getTimeIncrement)(IScopeHandle scope);
  	IIntStreamHandle (*getTimestamp)(IScopeHandle scope);
  	void (*clear)(IScopeHandle scope);
	void (*configureTrigger)(IScopeHandle scope, TriggerConfiguration conf);
	void (*configureChannelAddress)(IScopeHandle scope, void* address,
	                                uint32_t idOfChangedChannel, DATA_TYPES typeOfAddress);

	void (*setChannelRunning)(IScopeHandle scope, uint32_t idOfChangedChannel);
	void (*setChannelStopped)(IScopeHandle scope, uint32_t idOfChangedChannel);
	size_t (*getAmountOfChannels)(IScopeHandle scope);

} IScope ;

#endif
