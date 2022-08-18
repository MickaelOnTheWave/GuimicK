#include "telegramrunningbot.h"

#include <tgbot/tgbot.h>

namespace  {

}

TelegramRunningBot::TelegramRunningBot(const std::string& _botToken)
   : RunningBot(_botToken)
{
}

void TelegramRunningBot::LoopRun()
{
   TgBot::Bot bot(botToken);
   bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {
       bot.getApi().sendMessage(message->chat->id, "I started a maintenance run.");
   });
   bot.getEvents().onAnyMessage([&bot, this](TgBot::Message::Ptr message)
   {
      auto itCommand = validCommands.find(message->text);
      if (itCommand != validCommands.end())
      {
         bot.getApi().sendMessage(message->chat->id, itCommand->second.description);
      }
      else
         bot.getApi().sendMessage(message->chat->id, "Unknown command : " + message->text);
      printf("User wrote %s\n", message->text.c_str());
       if (StringTools::startsWith(message->text, "/start")) {
           return;
       }
   });

   try {
       printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
       TgBot::TgLongPoll longPoll(bot);
       while (true) {
           printf("Long poll started\n");
           longPoll.start();
       }
   } catch (TgBot::TgException& e) {
       printf("error: %s\n", e.what());
   }
}
