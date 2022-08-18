#ifndef BOTFACTORY_H
#define BOTFACTORY_H

#include "clientworkmanager.h"
#include "runningbot.h"
#include "SelfIdentity.h"

class BotFactory
{
public:
   static RunningBot* Create(Agent* agent, ClientWorkManager* workList, const std::string& botToken);
};

#endif // BOTFACTORY_H
