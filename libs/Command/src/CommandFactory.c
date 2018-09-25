/*!****************************************************************************************************************************************
 * @file         CommandFactory.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Command/CommandFactory.h>
#include <string.h>


/* Define public data */
typedef struct __CommandFactoryPrivateData
{
  CommandRunningHandle commandRunning;
  CommandPollHandle commandPoll;
  
} CommandFactoryPrivateData ;

/* Public functions */
CommandFactoryHandle CommandFactory_create(IScopeHandle iScope, 
                                           ChannelHandle* channels, 
                                           size_t ammountOfChannels){

  CommandFactoryHandle self = malloc(sizeof(CommandFactoryPrivateData));
  self->commandRunning = CommandRunning_create(channels, ammountOfChannels);
  self->commandPoll = CommandPoll_create(iScope);

  return self;
}

ICommandHandle CommandFactory_getICommand(CommandFactoryHandle self, char* command){

  if(strcpy(command, CommandRunning_getName(self->commandRunning)) == 0){
    return CommandRunning_getICommand(self->commandRunning);
  } else if(strcpy(command, CommandPoll_getName(self->commandPoll)) == 0){
    return CommandPoll_getICommand(self->commandPoll);
  }
  
  return NULL;
}

void CommandFactory_destroy(CommandFactoryHandle self){
  free(self);
}
