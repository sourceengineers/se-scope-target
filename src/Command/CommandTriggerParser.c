/*!****************************************************************************************************************************************
 * @file         CommandTriggerParser.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Command/CommandTriggerParser.h>
#include <string.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Class data */
typedef struct __CommandTriggerParserPrivateData
{
  ICommandHandle command;
  IUnpackerHandle unpacker;
  char* commandName;
  ChannelHandle* channels;
  size_t numberOfChannels;

} CommandTriggerParserPrivateData ;

/* Parses the given string into the correct trigger mode. Default is continious */
static TRIGGER_MODE parseStringToTriggerMode(const char* triggerModeString, size_t maxStringSize);

/* Parses the given string into a trigger edge. Default is rising edge */
static int parseStringToEdge(const char* triggerModeString, size_t maxStringSize);

/******************************************************************************
 Private functions
******************************************************************************/
static TRIGGER_MODE parseStringToTriggerMode(const char* triggerModeString, size_t maxStringSize){

  if(strncmp(triggerModeString, "Continous", maxStringSize) == 0){
    return TRIGGER_CONTINUOUS;
  } else if(strncmp(triggerModeString, "Normal", maxStringSize) == 0){
    return TRIGGER_NORMAL;
  } else if(strncmp(triggerModeString, "OneShot", maxStringSize) == 0){
    return TRIGGER_ONESHOT;
  }

  return TRIGGER_CONTINUOUS;
}

static int parseStringToEdge(const char* edgeToParse, size_t maxStringSize){

  if(strncmp(edgeToParse, "rising", maxStringSize) == 0){
    return TRIGGER_EDGE_POSITIVE;
  } else if(strncmp(edgeToParse, "falling", maxStringSize) == 0){
    return TRIGGER_EDGE_NEGATIVE;
  }

  return TRIGGER_EDGE_POSITIVE;
}

/******************************************************************************
 Public functions
******************************************************************************/
CommandTriggerParserHandle CommandTriggerParser_create(ICommandHandle command, IUnpackerHandle unpacker,
                                                       ChannelHandle* channels, size_t numberOfChannels){
  CommandTriggerParserHandle self = malloc(sizeof(CommandTriggerParserPrivateData));
  self->command = command;
  self->unpacker = unpacker;
  self->commandName = (char*) self->command->getCommandName(self->command);
  self->channels = channels;
  self->numberOfChannels = numberOfChannels;

  return self;
}

void CommandTriggerParser_configure(CommandTriggerParserHandle self){

  if(self->unpacker == NULL){
    return;
  }

  const size_t maxStringSize = 20;

  CommandFetchingInformation information = { .commandName = self->commandName, .fieldName = (char*) "cl_id",
                                             .isInArray = false, .arrayIndex = 0 };

  /* Get the correct data stream */
  gemmi_uint channelId = self->unpacker->getIntFromCommand(self->unpacker, &information);

  if(channelId >= self->numberOfChannels){
    channelId = 0;
  }

  IFloatStream stream = *Channel_getTriggerDataStream(self->channels[channelId]);

  /* Get the trigger level */
  information.fieldName = (char*) "level";
  const float level =  self->unpacker->getFloatFromCommand(self->unpacker, &information);

  /* Get the trigger mode */
  char triggerModeToParse[maxStringSize];
  information.fieldName = (char*) "mode";
  self->unpacker->getStringFromCommand(self->unpacker, &information, triggerModeToParse, maxStringSize);
  TRIGGER_MODE mode = parseStringToTriggerMode(triggerModeToParse, maxStringSize);

  /* Get the trigger edge */
  char edgeToParse[maxStringSize];
  information.fieldName = (char*) "edge";
  self->unpacker->getStringFromCommand(self->unpacker, &information, edgeToParse, maxStringSize);
  int edge = parseStringToEdge(edgeToParse, maxStringSize);

  TriggerConfiguration conf = {.level = level, .mode = mode, .stream = stream, .edge = edge, .channelId = channelId};

  self->command->setCommandAttribute(self->command, (void*) &conf);
}

void CommandTriggerParser_destroy(CommandTriggerParserHandle self){
  free(self);
  self = NULL;
}
