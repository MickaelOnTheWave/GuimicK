#ifndef RUNNINGBOT_H
#define RUNNINGBOT_H

#include <map>
#include <string>

class CommandData
{
public:
   CommandData() : description("") {}
   CommandData(const std::string& _description) : description(_description) {}

   std::string description;
};

class RunningBot
{
public:
   RunningBot(const std::string& _botToken);
   virtual ~RunningBot();

   virtual void LoopRun() = 0;

protected:
   std::string botToken;
   std::map<std::string, CommandData> validCommands;
};

#endif // RUNNINGBOT_H
