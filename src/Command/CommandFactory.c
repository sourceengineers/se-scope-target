/*!****************************************************************************************************************************************
 * @file         CommandFactory.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Command/CommandFactory.h>
#include <string.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandFactoryPrivateData
{
  CommandRunningHandle commandRunning;
  CommandPollHandle commandPoll;
  CommandAddrHandle commandAddr;
  CommandTIncHandle commandTInc;
  CommandTransHandle commandTrans;
  CommandTriggerHandle commandTrigger;
  CommandAnnounceHandle commandAnnounce;

  CommandAddrParserHandle commandAddrParser;
  CommandRunningParserHandle commandRunningParser;
  CommandTIncParserHandle commandTIncParser;
  CommandTriggerParserHandle commandTriggerParser;
  CommandPollParserHandle commandPollParser;

} CommandFactoryPrivateData ;


/******************************************************************************
 Public functions
******************************************************************************/
CommandFactoryHandle CommandFactory_create(IScopeHandle iScope,
                                           ChannelHandle* channels, 
                                           size_t amountOfChannels,
                                           TriggerHandle trigger,
                                           IUnpackerHandle unpacker){

  CommandFactoryHandle self = malloc(sizeof(CommandFactoryPrivateData));
  /* Initialize commands */
  self->commandRunning = CommandRunning_create(channels, amountOfChannels);
  self->commandPoll = CommandPoll_create(iScope);
  self->commandAddr = CommandAddr_create(channels, amountOfChannels);
  self->commandTInc = CommandTInc_create(iScope);
  self->commandTrans = CommandTrans_create(iScope);
  self->commandTrigger = CommandTrigger_create(trigger);
  self->commandAnnounce = CommandAnnounce_create(iScope);

  /* Initialize needed command parser
   * Not all commands need parser, which is why there are less parser as commands */
  self->commandAddrParser = CommandAddrParser_create(CommandAddr_getICommand(self->commandAddr), unpacker);

  self->commandRunningParser = CommandRunningParser_create(CommandRunning_getICommand(self->commandRunning), unpacker);

  self->commandTIncParser = CommandTIncParser_create(CommandTInc_getICommand(self->commandTInc), unpacker);

  self->commandTriggerParser = CommandTriggerParser_create(CommandTrigger_getICommand(self->commandTrigger), unpacker,
                                                           channels, amountOfChannels);

  self->commandPollParser = CommandPollParser_create(CommandPoll_getICommand(self->commandPoll), unpacker);

  return self;
}

ICommandHandle CommandFactory_getICommand(CommandFactoryHandle self, const char* command){

  if(strcmp(command, CommandRunning_getName(self->commandRunning)) == 0){
    CommandRunningParser_configure(self->commandRunningParser);
    return CommandRunning_getICommand(self->commandRunning);

  } else if(strcmp(command, CommandPoll_getName(self->commandPoll)) == 0){
    CommandPollParser_configure(self->commandPollParser);
    return CommandPoll_getICommand(self->commandPoll);

  } else if(strcmp(command, CommandAddr_getName(self->commandAddr)) == 0) {
    CommandAddrParser_configure(self->commandAddrParser);
    return CommandAddr_getICommand(self->commandAddr);

  } else if(strcmp(command, CommandTInc_getName(self->commandTInc)) == 0){
    CommandTIncParser_configure(self->commandTIncParser);
    return CommandTInc_getICommand(self->commandTInc);

  } else if(strcmp(command, CommandTrans_getName(self->commandTrans)) == 0){
    return CommandTrans_getICommand(self->commandTrans);

  } else if(strcmp(command, CommandTrigger_getName(self->commandTrigger)) == 0){
    CommandTriggerParser_configure(self->commandTriggerParser);
    return CommandTrigger_getICommand(self->commandTrigger);

  } else if(strcmp(command, CommandAnnounce_getName(self->commandAnnounce)) == 0){
    return CommandAnnounce_getICommand(self->commandAnnounce);
  }
  
  return NULL;
}

void CommandFactory_destroy(CommandFactoryHandle self){

  CommandRunning_destroy(self->commandRunning);
  CommandPoll_destroy(self->commandPoll);
  CommandAddr_destroy(self->commandAddr);
  CommandTInc_destroy(self->commandTInc);
  CommandTrans_destroy(self->commandTrans);
  CommandTrigger_destroy(self->commandTrigger);
  CommandAnnounce_destroy(self->commandAnnounce);

  CommandRunningParser_destroy(self->commandRunningParser);
  CommandAddrParser_destroy(self->commandAddrParser);
  CommandTIncParser_destroy(self->commandTIncParser);
  CommandTriggerParser_destroy(self->commandTriggerParser);
  CommandPollParser_destroy(self->commandPollParser);

  free(self);
  self = NULL;
}
