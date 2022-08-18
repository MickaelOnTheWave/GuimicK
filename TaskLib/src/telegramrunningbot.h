#ifndef TELEGRAMRUNNINGBOT_H
#define TELEGRAMRUNNINGBOT_H

#include <string>
#include <vector>

#include "runningbot.h"

class TelegramRunningBot : public RunningBot
{
public:
   TelegramRunningBot(const std::string& _botToken);

   void LoopRun() override;

private:
   bool IsValidCommand(const std::string& message);

   std::vector<std::string> validCommands;
};

#endif // TELEGRAMRUNNINGBOT_H
