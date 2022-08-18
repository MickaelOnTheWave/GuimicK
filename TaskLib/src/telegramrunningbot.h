#ifndef TELEGRAMRUNNINGBOT_H
#define TELEGRAMRUNNINGBOT_H

#include <string>
#include <vector>

class TelegramRunningBot
{
public:
   TelegramRunningBot();

   void Run();

private:
   bool IsValidCommand(const std::string& message);

   std::string botToken;
   std::vector<std::string> validCommands;
};

#endif // TELEGRAMRUNNINGBOT_H
