#ifndef BOTFACTORY_H
#define BOTFACTORY_H

#include "Bot/runningbot.h"
#include "WorkExecutionManager.h"

class BotFactory
{
public:
   static RunningBot* Create(WorkExecutionManager& data);
};

#endif // BOTFACTORY_H
