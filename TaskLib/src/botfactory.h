#ifndef BOTFACTORY_H
#define BOTFACTORY_H

#include "runningbot.h"

class BotFactory
{
public:
   static RunningBot* Create(const int botMode);
};

#endif // BOTFACTORY_H
