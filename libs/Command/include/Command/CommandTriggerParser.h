/*!*****************************************************************************
 * @file         CommandTrigger.h
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 * @brief        Implementation of the configurator for the cf_Trigger.
 *
 *              
 ******************************************************************************/
 
#ifndef COMMANDTRIGGERPARSER_H_
#define COMMANDTRIGGERPARSER_H_

#include <Command/ICommand.h>
#include <Communication/Reciever.h>
#include <Command/CommandTrigger.h>


/* Defines class */
typedef struct __CommandTriggerParserPrivateData* CommandTriggerParserHandle;

/******************************************************************************
 Public functions 
******************************************************************************/
/* Constructor: Creates a new instanze of the channel */
CommandTriggerParserHandle CommandTriggerParser_create(ICommandHandle iCommand, IUnpackerHandle iUnpacker,
                                                       ChannelHandle* channels, size_t numberOfChannels);

/* Configurates the gives channel */
void CommandTriggerParser_configure(CommandTriggerParserHandle self);

/* Deconstructor: Deletes the instanze of the channel */
void CommandTriggerParser_destroy(CommandTriggerParserHandle self);

#endif
