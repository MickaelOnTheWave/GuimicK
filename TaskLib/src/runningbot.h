#ifndef RUNNINGBOT_H
#define RUNNINGBOT_H

#include <string>

class RunningBot
{
public:
   RunningBot(const std::string& _botToken);

   virtual void LoopRun() = 0;

protected:
   std::string botToken;
};

#endif // RUNNINGBOT_H
