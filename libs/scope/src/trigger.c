/*!*****************************************************************************
 * @file         trigger.c
 *
 * @copyright    Copyright (c) 2018 by Sourceengineers. All Rights Reserved.
 *
 * @authors      Samuel Schuepbach samuel.schuepbach@sourceengineers.com
 * 
 ******************************************************************************/

#include <scope/trigger.h>
#include <math.h>

/* Define public data */
struct TriggerPrivateStruct
{
  float level;
  int sign;
  int edge;
  ssize_t triggerIndex;
  Channel* channel;
  TriggerStrategy triggerStrategies[3];
  
  TriggerStrategy (*getTriggerStrategy)(Trigger* self, TR_MODE mode);
  void (*writeConfig)(Trigger* self, TriggerConfiguration conf);
  bool (*checkCurrentData)(Trigger* self, float* triggerData);
};

Trigger* Trigger_create(){

  Trigger* self = malloc(sizeof(Trigger));
  self->_private = malloc(sizeof(TriggerPrivate));
  
  /* Writing all strategies into a array to easier load it when the trigger 
     is reconfigured */
  self->_private->triggerStrategies[TR_CONTINUOUS] = &Trigger_continuous;
  self->_private->triggerStrategies[TR_NORMAL] = &Trigger_normal;
  self->_private->triggerStrategies[TR_ONESHOT] = &Trigger_oneShot;
  
  /* Attach private functions */
  self->_private->getTriggerStrategy = &Trigger_getTriggerStrategy;
  self->_private->writeConfig = &Trigger_writeConfig;
  self->_private->checkCurrentData = &Trigger_checkCurrentData;

  /* Attach public functions */
  self->configure = &Trigger_configure;
  self->trigger = &Trigger_continuous;
  self->getTriggerIndex = &Trigger_getTriggerIndex;
  
  return self;
}

void Trigger_destroy(Trigger* self){
  free(self->_private);
  free(self);
}

static bool Trigger_configSanityCheck(TriggerConfiguration conf){
  if((conf.mode != TR_NORMAL) \ 
      && (conf.mode != TR_CONTINUOUS) \ 
      && (conf.mode != TR_ONESHOT)){
    return false;
  }
  if((conf.edge != TR_EDGE_NEGATIVE) \
      && (conf.edge != TR_EDGE_POSITIVE)){
    return false;
  }
  if(conf.channel == NULL){
    return false;
  }
  return true;
}

static TriggerStrategy Trigger_getTriggerStrategy(Trigger* self, TR_MODE mode){
  return self->_private->triggerStrategies[mode];
}

static void Trigger_writeConfig(Trigger* self, TriggerConfiguration conf){
  self->_private->level = conf.level;
  self->_private->edge = conf.edge;
  self->_private->sign = (int) copysign(1.0f, conf.level);
  self->_private->channel = conf.channel;
  self->trigger = self->_private->getTriggerStrategy(self, conf.mode);
}

static int Trigger_getTriggerIndex(Trigger* self){
    return (int) self->_private->triggerIndex;
}

static bool Trigger_configure(Trigger* self, TriggerConfiguration conf){
    
    if(Trigger_configSanityCheck(conf) == false){
      return false;
    }
    self->_private->writeConfig(self, conf);
    return true;
}

static bool Trigger_checkCurrentData(Trigger* self, float* triggerData){
  
  const float dataCurrent = triggerData[CL_CURRENT_DATA];
  const float dataLast = triggerData[CL_OLD_DATA];
  const float triggerLevel = self->_private->level;
  const int edge = (const int) dataCurrent > dataLast ? 1 : -1;
  const int sign = (const int) copysign(1.0f, dataCurrent);
  
  if(dataCurrent == dataLast){
    return false;
  }
  
  if(edge != self->_private->edge){
    return false;
  }
  
  /* Sort the two values to check if the triggerLevel is between them */
  const float levelLower = dataCurrent > dataLast ? dataLast : dataCurrent;
  const float levelHigher = dataCurrent > dataLast ? dataCurrent : dataLast;
  if(triggerLevel < levelLower || triggerLevel > levelHigher){
    return false;
  }

  return true;
}

static bool Trigger_continuous(Trigger* self, const int index){
  self->_private->triggerIndex = -1;
  return false;
}

static bool Trigger_normal(Trigger* self, const int index){
  
  Channel* channel = self->_private->channel;
  
  float triggerData[2];
  channel->getTriggerData(channel, triggerData);

  const bool isTriggered = self->_private->checkCurrentData(self, triggerData);
  
  if(isTriggered == true){
    self->_private->triggerIndex = index;
  }
  
  return isTriggered;
}

static bool Trigger_oneShot(Trigger* self, const int index){
  return false;
}
