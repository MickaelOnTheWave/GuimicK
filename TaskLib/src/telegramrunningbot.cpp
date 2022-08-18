#include "telegramrunningbot.h"

#include <tgbot/tgbot.h>

namespace  {

   void ExecuteCommand(TgBot::Bot& bot, TgBot::Message::Ptr message)
   {
      const std::string command = message->text;
      if (command == "/help")
      {
         bot.getApi().sendMessage(message->chat->id, "Here are the available commands :\n"
                                                     "   /help\n"
                                                     "   /listjobs");

      }
      else if (command == "/listjobs")
      {
         bot.getApi().sendMessage(message->chat->id, "Here are the jobs scheduled to run :");
      }
      else if (command == "/noshutdown")
      {
         bot.getApi().sendMessage(message->chat->id, "Ok, I will keep running even when I finish my jobs, waiting for your commands.");
      }
      else if (command == "/shutdownnow")
      {
         bot.getApi().sendMessage(message->chat->id, "Shutting down now...");
         bot.getApi().sendMessage(message->chat->id, "Bye!");
      }
   }

}

TelegramRunningBot::TelegramRunningBot(const std::string& _botToken)
   : RunningBot(_botToken)
{
   validCommands.push_back("/help");
   validCommands.push_back("/listjobs");
   validCommands.push_back("/noshutdown");
   validCommands.push_back("/shutdownnow");
}

void TelegramRunningBot::LoopRun()
{
   TgBot::Bot bot(botToken);
   bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {
       bot.getApi().sendMessage(message->chat->id, "I started a maintenance run.");
   });
   bot.getEvents().onAnyMessage([&bot, this](TgBot::Message::Ptr message) {
      if (IsValidCommand(message->text))
         ExecuteCommand(bot, message);
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

bool TelegramRunningBot::IsValidCommand(const std::string& message)
{
   for (auto validCommand : validCommands)
   {
      if (message.find(validCommand) == 0)
         return true;
   }
   return false;
}

