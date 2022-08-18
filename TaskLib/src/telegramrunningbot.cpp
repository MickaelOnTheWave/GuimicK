#include "telegramrunningbot.h"

TelegramRunningBot::TelegramRunningBot(Agent* _agent, ClientWorkManager* _worklist, const std::string& token)
   : RunningBot(_agent, _worklist),
     bot(token)
{
}

void TelegramRunningBot::LoopRun()
{
   bot.getEvents().onCommand("start", [this](TgBot::Message::Ptr message) {
       bot.getApi().sendMessage(message->chat->id, "I started a maintenance run.");
   });
   bot.getEvents().onAnyMessage([this](TgBot::Message::Ptr message)
   {
      auto itCommand = validCommands.find(message->text);
      if (itCommand != validCommands.end())
      {
         currentMessage = message;
         itCommand->second.functor();
      }
      else
         bot.getApi().sendMessage(message->chat->id, "Unknown command : " + message->text);

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

void TelegramRunningBot::SendMessage(const std::string& message) const
{
   bot.getApi().sendMessage(currentMessage->chat->id, message);
}

void TelegramRunningBot::ExecuteGiveUserId()
{
   std::string message = std::string("Hey ") + currentMessage->from->firstName + ", your id is ";
   message += std::to_string(currentMessage->from->id);
   bot.getApi().sendMessage(currentMessage->chat->id, message);
}
