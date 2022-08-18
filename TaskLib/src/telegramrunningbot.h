#ifndef TELEGRAMRUNNINGBOT_H
#define TELEGRAMRUNNINGBOT_H

#include <string>
#include <tgbot/tgbot.h>

#include "runningbot.h"

class TelegramRunningBot : public RunningBot
{
public:
   TelegramRunningBot(Agent* _agent, ClientWorkManager* _worklist, const std::string& token);

   void LoopRun() override;

protected:
   void SendMessage(const std::string& message) const override;
   void ExecuteGiveUserId() override;

private:
   TgBot::Bot bot;
   TgBot::Message::Ptr currentMessage;
};

#endif // TELEGRAMRUNNINGBOT_H
