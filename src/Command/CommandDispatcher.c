/*!****************************************************************************************************************************************
 * @file         CommandDispatcher.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Command/CommandDispatcher.h>
#include <string.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandDispatcherPrivateData
{
  CommandRunningHandle commandRunning;
  CommandPollHandle commandPoll;
  CommandAddrHandle commandAddr;
  CommandClearHandle commandClear;
  CommandTIncHandle commandTInc;
  CommandTransHandle commandTrans;
  CommandTriggerHandle commandTrigger;
  CommandAnnounceHandle commandAnnounce;

  CommandAddrParserHandle commandAddrParser;
  CommandRunningParserHandle commandRunningParser;
  CommandTIncParserHandle commandTIncParser;
  CommandTriggerParserHandle commandTriggerParser;
  CommandPollParserHandle commandPollParser;

} CommandDispatcherPrivateData ;


/******************************************************************************
 Public functions
******************************************************************************/
CommandDispatcherHandle CommandDispatcher_create(IScopeHandle scope,
                                           ChannelHandle* channels, 
                                           size_t amountOfChannels,
                                           TriggerHandle trigger,
                                           IUnpackerHandle unpacker){

  CommandDispatcherHandle self = malloc(sizeof(CommandDispatcherPrivateData));
  /* Initialize commands */
  self->commandRunning = CommandRunning_create(channels, amountOfChannels);
  self->commandPoll = CommandPoll_create(scope);
  self->commandAddr = CommandAddr_create(channels, amountOfChannels);
  self->commandTInc = CommandTInc_create(scope);
  self->commandTrans = CommandTrans_create(scope);
  self->commandTrigger = CommandTrigger_create(trigger);
  self->commandAnnounce = CommandAnnounce_create(scope);
  self->commandClear = CommandClear_create(scope);

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

// das hier würde ich in eine klasse CommandParser auslagern so dass die factory nur die commands erzeugt
// die methode könnte CommandParser_run oder Commadnparser_parse heissen
ICommandHandle CommandDispatcher_getICommand(CommandDispatcherHandle self, const char* command){

  if(strncmp(command, CommandRunning_getName(self->commandRunning), MAX_COMMAND_LENGTH) == 0){
    CommandRunningParser_configure(self->commandRunningParser);
    return CommandRunning_getICommand(self->commandRunning);

  } else if(strncmp(command, CommandPoll_getName(self->commandPoll), MAX_COMMAND_LENGTH) == 0){
    CommandPollParser_configure(self->commandPollParser);
    return CommandPoll_getICommand(self->commandPoll);

  } else if(strncmp(command, CommandAddr_getName(self->commandAddr), MAX_COMMAND_LENGTH) == 0) {
    CommandAddrParser_configure(self->commandAddrParser);
    return CommandAddr_getICommand(self->commandAddr);

  } else if(strncmp(command, CommandTInc_getName(self->commandTInc), MAX_COMMAND_LENGTH) == 0){
    CommandTIncParser_configure(self->commandTIncParser);
    return CommandTInc_getICommand(self->commandTInc);

  } else if(strncmp(command, CommandTrans_getName(self->commandTrans), MAX_COMMAND_LENGTH) == 0){
    return CommandTrans_getICommand(self->commandTrans);

  } else if(strncmp(command, CommandTrigger_getName(self->commandTrigger), MAX_COMMAND_LENGTH) == 0){
    CommandTriggerParser_configure(self->commandTriggerParser);
    return CommandTrigger_getICommand(self->commandTrigger);

  } else if(strncmp(command, CommandAnnounce_getName(self->commandAnnounce), MAX_COMMAND_LENGTH) == 0){
    return CommandAnnounce_getICommand(self->commandAnnounce);

  } else if(strncmp(command, CommandClear_getName(self->commandClear), MAX_COMMAND_LENGTH) == 0){
    return CommandClear_getICommand(self->commandClear);
  }
  
  return NULL;
}

void CommandDispatcher_destroy(CommandDispatcherHandle self){

  CommandRunning_destroy(self->commandRunning);
  CommandPoll_destroy(self->commandPoll);
  CommandAddr_destroy(self->commandAddr);
  CommandTInc_destroy(self->commandTInc);
  CommandTrans_destroy(self->commandTrans);
  CommandTrigger_destroy(self->commandTrigger);
  CommandAnnounce_destroy(self->commandAnnounce);
  CommandClear_destroy(self->commandClear);

  CommandRunningParser_destroy(self->commandRunningParser);
  CommandAddrParser_destroy(self->commandAddrParser);
  CommandTIncParser_destroy(self->commandTIncParser);
  CommandTriggerParser_destroy(self->commandTriggerParser);
  CommandPollParser_destroy(self->commandPollParser);

  free(self);
  self = NULL;
}
