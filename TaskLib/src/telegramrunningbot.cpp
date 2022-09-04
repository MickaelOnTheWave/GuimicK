#include "telegramrunningbot.h"

TelegramRunningBot::TelegramRunningBot(Agent* _agent, ClientWorkManager* _worklist, const std::string& token)
   : RunningBot(_agent, _worklist),
     bot(_agent->GetBotData()->botToken)
{
   chatId = 5483529663;
}

void TelegramRunningBot::LoopRun()
{
   bot.getEvents().onCommand("", [this](TgBot::Message::Ptr message) {
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
   });

   try
   {
      OnStart();

      TgBot::TgLongPoll longPoll(bot);
      while (true)
         longPoll.start();

      OnFinish();
   } catch (TgBot::TgException& e) {
       printf("error: %s\n", e.what());
   }   
}

void TelegramRunningBot::SendMessage(const std::string& message) const
{
   bot.getApi().sendMessage(chatId, message);
}

bool TelegramRunningBot::IsUserAuthorized()
{
   const bool authorized = (std::to_string(currentMessage->from->id) == agent->GetBotData()->authorizedUserToken);
   if (!authorized)
      bot.getApi().sendMessage(currentMessage->chat->id, "I don't know you. Who are you?");
   return authorized;
}

void TelegramRunningBot::ShutdownBot()
{
   bot.getApi().leaveChat(chatId);
}

void TelegramRunningBot::ExecuteGiveUserId()
{
   std::string message = std::string("Hey ") + currentMessage->from->firstName + ", your id is ";
   message += std::to_string(currentMessage->from->id);
   bot.getApi().sendMessage(currentMessage->chat->id, message);
}

void TelegramRunningBot::ExecuteShowInfo()
{
   std::string message = std::string("Here is some internal data about myself :\n");
   message += std::string("\tTelegram Data about myself :\n");
   message += std::string("\t\tUsername :") + bot.getApi().getMe()->username + "\n";
   message += std::string("\t\tFirst name :") + bot.getApi().getMe()->firstName + "\n";
   message += std::string("\t\tLast name :") + bot.getApi().getMe()->lastName + "\n";
   message += std::string("\t\tid :") + std::to_string(bot.getApi().getMe()->id) + "\n";
   message += std::string("\t\tLanguage code :") + bot.getApi().getMe()->languageCode + "\n";
   message += std::string("\t\tAm I a bot :") + std::to_string(bot.getApi().getMe()->isBot) + "(You had a doubt? :-) )\n";
   message += std::string("\tTelegram Data about *YOU* :\n");
   message += std::string("\t\tUsername :") + currentMessage->from->username + "\n";
   message += std::string("\t\tFirst name :") + currentMessage->from->firstName + "\n";
   message += std::string("\t\tLast name :") + currentMessage->from->lastName + "\n";
   message += std::string("\t\tid :") + std::to_string(currentMessage->from->id) + "\n";
   message += std::string("\t\tAre you a bot? :") + std::to_string(currentMessage->from->isBot) + "\n";
   message += std::string("\tTelegram Data about us :\n");
   message += std::string("\t\tUsername :") + currentMessage->chat->username + "\n";
   message += std::string("\t\tFirst name :") + currentMessage->chat->firstName + "\n";
   message += std::string("\t\tLast name :") + currentMessage->chat->lastName + "\n";
   message += std::string("\t\tid :") + std::to_string(currentMessage->chat->id) + "\n";

   bot.getApi().sendMessage(currentMessage->chat->id, message);
}
