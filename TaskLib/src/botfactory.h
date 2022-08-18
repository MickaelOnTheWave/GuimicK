#ifndef BOTFACTORY_H
#define BOTFACTORY_H

#include "runningbot.h"

class BotFactory
{
public:
   static RunningBot* Create(const int botMode, const std::wstring& botToken);
};

#endif // BOTFACTORY_H
