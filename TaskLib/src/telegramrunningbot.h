#ifndef TELEGRAMRUNNINGBOT_H
#define TELEGRAMRUNNINGBOT_H

#include <string>

#include "runningbot.h"

class TelegramRunningBot : public RunningBot
{
public:
   TelegramRunningBot(const std::string& _botToken);

   void LoopRun() override;

private:
};

#endif // TELEGRAMRUNNINGBOT_H
