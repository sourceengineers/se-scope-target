/*!****************************************************************************************************************************************
 * @file         CommandTriggerParser.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 *
 *****************************************************************************************************************************************/

#include <Scope/Control/CommandParser/CommandTriggerParser.h>
#include <string.h>

/******************************************************************************
 Define private data
******************************************************************************/
/* Name of the command */
static char* commandName = "cf_tgr";

/* Class data */
typedef struct __CommandTriggerParserPrivateData
{
  CommandTriggerHandle command;
  IUnpackerHandle unpacker;

} CommandTriggerParserPrivateData ;

/* Parses the given string into the correct trigger mode. Default is continious */
static TRIGGER_MODE parseStringToTriggerMode(const char* triggerModeString, size_t maxStringSize);

/* Parses the given string into a trigger edge. Default is rising edge */
static int parseStringToEdge(const char* edgeToParse, size_t maxStringSize);

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
CommandTriggerParserHandle CommandTriggerParser_create(IScopeHandle scope, IUnpackerHandle unpacker){
  CommandTriggerParserHandle self = malloc(sizeof(CommandTriggerParserPrivateData));
  self->command = CommandTrigger_create(scope);
  self->unpacker = unpacker;

  return self;
}

ICommandHandle CommandTriggerParser_getCommand(CommandTriggerParserHandle self){

  if(self->unpacker == NULL){
    return NULL;
  }

  const size_t maxStringSize = 20;

  CommandFetchingInformation information = { .commandName = commandName, .fieldName = (char*) "cl_id",
                                             .isInArray = false, .arrayIndex = 0 };

  /* Get the correct data stream */
  const uint32_t channelId = self->unpacker->getIntFromCommand(self->unpacker, &information);

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

  TriggerConfiguration conf = {.level = level, .mode = mode, .edge = edge, .channelId = channelId};

  CommandTrigger_setAttributes(self->command, conf);

  return CommandTrigger_getICommand(self->command);
}

char* CommandTriggerParser_getName(){
  return commandName;
}
void CommandTriggerParser_destroy(CommandTriggerParserHandle self){
  CommandTrigger_destroy(self->command);

  free(self);
  self = NULL;
}
